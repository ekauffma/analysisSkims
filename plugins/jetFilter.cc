#include <memory>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <string>


class jetFilter: public edm::global::EDFilter<>
{
    public: 
        explicit jetFilter(const edm::ParameterSet&);
        ~jetFilter() override = default;
    
    private:
        void beginJob() override{};
        bool filter(edm::StreamID, edm::Event&, edm::EventSetup const&) const override;
        void endJob() override{};

        edm::EDGetTokenT<std::vector<pat::Jet>> objectSrc;
        edm::EDGetTokenT<std::vector<pat::Electron>> slimmedElectronsToken_;
        edm::EDGetTokenT<std::vector<pat::Muon>> slimmedMuonsToken_;

        int nJetThreshold;
        int nLeptonThreshold;
};

jetFilter::jetFilter(const edm::ParameterSet& iConfig):
    objectSrc(consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("objectSrc")))
{
    slimmedElectronsToken_ = consumes< std::vector<pat::Electron> >(edm::InputTag("slimmedElectrons") );
    slimmedMuonsToken_ = consumes< std::vector<pat::Muon> >(edm::InputTag("slimmedMuons") );
    nJetThreshold = 1;
    nLeptonThreshold = 1;

}

bool jetFilter::filter(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const
{
    using namespace edm;

    edm::Handle<std::vector<pat::Jet>> jetHandle;
    iEvent.getByToken(objectSrc, jetHandle);
    float nJets = jetHandle->size();

    edm::Handle<std::vector<pat::Electron>> slimmedElectrons;
    iEvent.getByToken(slimmedElectronsToken_, slimmedElectrons);
    edm::Handle<std::vector<pat::Muon>> slimmedMuons;
    iEvent.getByToken(slimmedMuonsToken_, slimmedMuons);
    float nElectrons = slimmedElectrons->size();
    float nMuons = slimmedMuons->size();

    return ((nJets >= (int)nJetThreshold) || ((nElectrons + nMuons) >= (int)nLeptonThreshold));

}

DEFINE_FWK_MODULE(jetFilter);
