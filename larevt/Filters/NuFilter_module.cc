////////////////////////////////////////////////////////////////////////
// Class:       NuFilter
// Plugin Type: filter (Unknown Unknown)
// File:        NuFilter_module.cc
//
// Pierre Granger (granger@apc.in2p3.fr)
// Generated at Fri Mar 24 12:30:27 2023 by Pierre Granger using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <memory>
#include <cstring>

#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCNeutrino.h"

namespace filter {
  class NuFilter;
}


class filter::NuFilter : public art::EDFilter {
public:
  explicit NuFilter(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  NuFilter(NuFilter const&) = delete;
  NuFilter(NuFilter&&) = delete;
  NuFilter& operator=(NuFilter const&) = delete;
  NuFilter& operator=(NuFilter&&) = delete;

  // Required functions.
  bool filter(art::Event& e) override;

private:
  std::vector<int> parse_modes(const std::vector<std::string> &modes);
  std::vector<int> parse_CCNC(const std::vector<std::string> &CCNC);
  std::string capitalize(const std::string &inputString);


  std::vector<int> ftargets;
  std::vector<int> fmodes;
  std::vector<int> fCCNC;
  double fminE;  // Lower bound on the energy in GeV
  double fmaxE;  // Higher bound on the energy in GeV

  // Declare member data here.

};


filter::NuFilter::NuFilter(fhicl::ParameterSet const& p)
  : EDFilter{p}
  , ftargets{p.get<std::vector<int>>("Targets", {})}
  , fmodes{parse_modes(p.get<std::vector<std::string>>("Modes", {}))}
  , fCCNC{parse_CCNC(p.get<std::vector<std::string>>("CCNC", {}))}
  , fminE{p.get<double>("MinE", 0)}
  , fmaxE{p.get<double>("MaxE", 1e10)}
  // More initializers here.
{
  // Call appropriate produces<>() functions here.
  // Call appropriate consumes<>() for any products to be retrieved by this module.
}

bool filter::NuFilter::filter(art::Event& e)
{
  // Implementation of required member function here.

  art::Handle< std::vector< simb::MCTruth > > mct_handle;
  e.getByLabel("generator", mct_handle);

  if(mct_handle.isValid() && mct_handle->size()) {
    auto const& nu = mct_handle->at(0);
    const simb::MCNeutrino &neutrino = nu.GetNeutrino();

    if(neutrino.Nu().E() < fminE){
      return false;
    }

    if(neutrino.Nu().E() > fmaxE){
      return false;
    }

    if(ftargets.size() != 0 && std::find(ftargets.begin(), ftargets.end(), neutrino.Target()) == ftargets.end()){
      return false;
    }

    if(fCCNC.size() != 0 && std::find(fCCNC.begin(), fCCNC.end(), neutrino.CCNC()) == fCCNC.end()){
      return false;
    }

    if(fmodes.size() != 0 && std::find(fmodes.begin(), fmodes.end(), neutrino.Mode()) == fmodes.end()){
      return false;
    }

    return true;
    
  }

  return false;
}

std::string filter::NuFilter::capitalize(const std::string &inputString){
  std::string capitalized(inputString);
  std::transform(capitalized.begin(), capitalized.end(), capitalized.begin(), ::toupper);
  return capitalized;
}

std::vector<int> filter::NuFilter::parse_modes(const std::vector<std::string> &modes){
  std::vector<int> parsed;
  std::map<std::string, int> mode_list;
  mode_list.insert(std::make_pair("QE", simb::kQE));
  mode_list.insert(std::make_pair("RES", simb::kRes));
  mode_list.insert(std::make_pair("DIS", simb::kDIS));
  mode_list.insert(std::make_pair("COH", simb::kCoh));

  for(const std::string &mode : modes){
    const std::string normalized = capitalize(mode);
    std::map<std::string, int>::iterator it = mode_list.find(normalized);
    if(it != mode_list.end()){
      parsed.push_back(it->second);
    }
    else{
      std::ostringstream msg;
      msg << "Parameter '" << normalized << "' not recognized for Modes argument. Ignoring it!\n";
      msg << "Available parameters are: [";
      for(const auto &elt : mode_list){
        msg << elt.first << ", ";
      }
      msg << "].";

      mf::LogWarning("NuFilter") << msg.str();
    }
  }

  return parsed;
}

std::vector<int> filter::NuFilter::parse_CCNC(const std::vector<std::string> &CCNC){
  std::vector<int> parsed;

  for (const std::string& item : CCNC){
    const std::string normalized = capitalize(item);
    int value;
    if(normalized == "CC"){
      value = simb::kCC;
    }
    else if(normalized == "NC"){
      value = simb::kNC;
    }
    else{
      mf::LogWarning("NuFilter") << "Parameter '" << item << "' not recognized for CCNC argument. Ignoring it!";
      continue;
    }
    parsed.push_back(value);
  }

  return parsed;
}

DEFINE_ART_MODULE(filter::NuFilter)
