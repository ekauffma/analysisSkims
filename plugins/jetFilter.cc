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
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <string>

#include "TLorentzVector.h"
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"


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

        TH1F* h_Selections;
};

jetFilter::jetFilter(const edm::ParameterSet& iConfig):
    objectSrc(consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("objectSrc")))
{
    slimmedElectronsToken_ = consumes< std::vector<pat::Electron> >(edm::InputTag("slimmedElectrons") );
    slimmedMuonsToken_ = consumes< std::vector<pat::Muon> >(edm::InputTag("slimmedMuons") );
    nJetThreshold = 1;
    nLeptonThreshold = 1;

    edm::Service<TFileService> fs;
    h_Selections = fs->make<TH1F>("h_Selections", "h_Selections", 2, -0.5, 1.5 );
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

    bool filterResult = ((nJets >= (int)nJetThreshold) || ((nElectrons + nMuons) >= (int)nLeptonThreshold));

    if (filterResult) h_Selections->Fill(1.0);
    else h_Selections->Fill(0.0);

    return filterResult;

}

DEFINE_FWK_MODULE(jetFilter);
