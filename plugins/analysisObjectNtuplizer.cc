#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/L1CaloTrigger/interface/CICADA.h"

#include "PhysicsTools/PatUtils/interface/bJetSelector.h"

using namespace std;

class analysisObjectNtuplizer: public edm::one::EDAnalyzer< edm::one::SharedResources>
{
    public:
        explicit analysisObjectNtuplizer(const edm::ParameterSet&);
        ~analysisObjectNtuplizer() override = default;

    private:
        void beginJob() override {};
        void endJob() override {};

        void analyze(const edm::Event&, const edm::EventSetup&) override;

        edm::EDGetTokenT<std::vector<pat::Jet>> objectSrc;
        edm::EDGetTokenT<vector<pat::Electron>> slimmedElectronsToken_;
        edm::EDGetTokenT<vector<pat::Muon>> slimmedMuonsToken_;
        edm::EDGetTokenT<vector<pat::Tau>> slimmedTausToken_;
        edm::EDGetTokenT<vector<pat::MET>> slimmedMETsToken_;
        edm::EDGetTokenT<l1t::CICADABxCollection> caloLayer1CICADAScoreToken_;
        edm::Service<TFileService> theFileService;

        TTree* outputTree;
        unsigned int jet_nObjects = 0;
        std::vector<double> jet_pt;
        std::vector<double> jet_eta;
        std::vector<double> jet_phi;
        std::vector<double> jet_mass;
        std::vector<double> jet_et;
        std::vector<int> jet_charge;
        std::vector<double> jet_mt;
        std::vector<double> jet_vx;
        std::vector<double> jet_vy;
        std::vector<double> jet_vz;
        std::vector<size_t> jet_nDaughters;
        std::vector<double> jet_vtxChi2;
        std::vector<double> jet_btagDeepCSV;

        unsigned int electron_nObjects = 0;
        std::vector<double> electron_pt;
        std::vector<double> electron_eta;
        std::vector<double> electron_phi;
        std::vector<double> electron_mass;
        std::vector<double> electron_et;
        std::vector<int> electron_charge;

        unsigned int muon_nObjects = 0;
        std::vector<double> muon_pt;
        std::vector<double> muon_eta;
        std::vector<double> muon_phi;
        std::vector<double> muon_mass;
        std::vector<double> muon_et;
        std::vector<int> muon_charge;

        unsigned int tau_nObjects = 0;
        std::vector<double> tau_pt;
        std::vector<double> tau_eta;
        std::vector<double> tau_phi;
        std::vector<double> tau_mass;
        std::vector<double> tau_et;
        std::vector<int> tau_charge;

        std::vector<double> MET_pt;
        std::vector<double> MET_eta;
        std::vector<double> MET_phi;

        double cicadaScore;


};

analysisObjectNtuplizer::analysisObjectNtuplizer(const edm::ParameterSet& iConfig):
    objectSrc(consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("objectSrc"))),
    caloLayer1CICADAScoreToken_(consumes<l1t::CICADABxCollection>(iConfig.getParameter<edm::InputTag>("caloLayer1CICADAScore")))
{
    slimmedElectronsToken_ = consumes< std::vector<pat::Electron> >(edm::InputTag("slimmedElectrons") );
    slimmedMuonsToken_ = consumes< std::vector<pat::Muon> >(edm::InputTag("slimmedMuons") );
    slimmedTausToken_ = consumes< std::vector<pat::Tau> >(edm::InputTag("slimmedTaus") );
    slimmedMETsToken_ = consumes< std::vector<pat::MET> >(edm::InputTag("slimmedMETs") );

    usesResource("TFileService");
    
    outputTree = theFileService->make<TTree>("PATObjects", "PAT Objects Information");

    outputTree->Branch("jet_nObjects", &jet_nObjects);
    outputTree->Branch("jet_pt", &jet_pt);
    outputTree->Branch("jet_eta", &jet_eta);
    outputTree->Branch("jet_phi", &jet_phi);
    outputTree->Branch("jet_mass", &jet_mass);
    outputTree->Branch("jet_et", &jet_et);
    outputTree->Branch("jet_charge", &jet_charge);
    outputTree->Branch("jet_mt", &jet_mt);
    outputTree->Branch("jet_vx", &jet_vx);
    outputTree->Branch("jet_vy", &jet_vy);
    outputTree->Branch("jet_vz", &jet_vz);
    outputTree->Branch("jet_nDaughters", &jet_nDaughters);
    outputTree->Branch("jet_vtxChi2", &jet_vtxChi2);
    outputTree->Branch("jet_btagDeepCSV", &jet_btagDeepCSV);

    outputTree->Branch("electron_nObjects", &electron_nObjects);
    outputTree->Branch("electron_pt", &electron_pt);
    outputTree->Branch("electron_eta", &electron_eta);
    outputTree->Branch("electron_phi", &electron_phi);
    outputTree->Branch("electron_mass", &electron_mass);
    outputTree->Branch("electron_et", &electron_et);
    outputTree->Branch("electron_charge", &electron_charge);

    outputTree->Branch("muon_nObjects", &muon_nObjects);
    outputTree->Branch("muon_pt", &muon_pt);
    outputTree->Branch("muon_eta", &muon_eta);
    outputTree->Branch("muon_phi", &muon_phi);
    outputTree->Branch("muon_mass", &muon_mass);
    outputTree->Branch("muon_et", &muon_et);
    outputTree->Branch("muon_charge", &muon_charge);

    outputTree->Branch("tau_nObjects", &tau_nObjects);
    outputTree->Branch("tau_pt", &tau_pt);
    outputTree->Branch("tau_eta", &tau_eta);
    outputTree->Branch("tau_phi", &tau_phi);
    outputTree->Branch("tau_mass", &tau_mass);
    outputTree->Branch("tau_et", &tau_et);
    outputTree->Branch("tau_charge", &tau_charge);

    outputTree->Branch("MET_pt", &MET_pt);
    outputTree->Branch("MET_eta", &MET_eta);
    outputTree->Branch("MET_phi", &MET_phi);

    outputTree->Branch("cicadaScore", &cicadaScore);
}

void analysisObjectNtuplizer::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{
    edm::Handle<std::vector<pat::Jet>> jetHandle;
    iEvent.getByToken(objectSrc, jetHandle);

    edm::Handle< std::vector<pat::Electron> > slimmedElectrons;
    iEvent.getByToken(slimmedElectronsToken_,slimmedElectrons );
    edm::Handle< std::vector<pat::Muon> > slimmedMuons;
    iEvent.getByToken(slimmedMuonsToken_,slimmedMuons );
    edm::Handle< std::vector<pat::Tau> > slimmedTaus;
    iEvent.getByToken(slimmedTausToken_,slimmedTaus );
    edm::Handle< std::vector<pat::MET> > slimmedMETs;
    iEvent.getByToken(slimmedMETsToken_,slimmedMETs );
    
    cicadaScore = (double)iEvent.get(caloLayer1CICADAScoreToken_)[0];

    jet_nObjects = jetHandle->size();
    for(
        auto theObject = jetHandle->begin();
        theObject != jetHandle->end();
        ++theObject
    )
    {
        jet_pt.push_back(theObject->pt());
        jet_eta.push_back(theObject->eta());
        jet_phi.push_back(theObject->phi());
        jet_mass.push_back(theObject->mass());
        jet_et.push_back(theObject->et());
        jet_charge.push_back(theObject->charge());
        jet_mt.push_back(theObject->mt());
        jet_vx.push_back(theObject->vx());
        jet_vy.push_back(theObject->vy());
        jet_vz.push_back(theObject->vz());
        jet_nDaughters.push_back(theObject->numberOfDaughters());
        jet_vtxChi2.push_back(theObject->vertexChi2());
        jet_btagDeepCSV.push_back(theObject->bDiscriminator("pfDeepCSVJetTags:probb"));
    }

    electron_nObjects = slimmedElectrons->size();
    for(
        auto theObject = slimmedElectrons->begin();
        theObject != slimmedElectrons->end();
        ++theObject
    )
    {
       electron_pt.push_back(theObject->pt());
       electron_eta.push_back(theObject->eta());
       electron_phi.push_back(theObject->phi());
       electron_mass.push_back(theObject->mass());
       electron_et.push_back(theObject->et());
       electron_charge.push_back(theObject->charge());
    }

    muon_nObjects = slimmedMuons->size();
    for(
        auto theObject = slimmedMuons->begin();
        theObject != slimmedMuons->end();
        ++theObject
    )
    {
       muon_pt.push_back(theObject->pt());
       muon_eta.push_back(theObject->eta());
       muon_phi.push_back(theObject->phi());
       muon_mass.push_back(theObject->mass());
       muon_et.push_back(theObject->et());
       muon_charge.push_back(theObject->charge());
    }                             

    tau_nObjects = slimmedTaus->size();
    for(
        auto theObject = slimmedTaus->begin();
        theObject != slimmedTaus->end();
        ++theObject
    )
    {
       tau_pt.push_back(theObject->pt());
       tau_eta.push_back(theObject->eta());
       tau_phi.push_back(theObject->phi());
       tau_mass.push_back(theObject->mass());
       tau_et.push_back(theObject->et());
       tau_charge.push_back(theObject->charge());
    }

    for(
        auto theObject = slimmedMETs->begin();
        theObject != slimmedMETs->end();
        ++theObject
    )
    {
       MET_pt.push_back(theObject->pt());
       MET_eta.push_back(theObject->eta());
       MET_phi.push_back(theObject->phi());
    }

    outputTree->Fill();

    jet_nObjects = 0;
    jet_pt.clear();
    jet_eta.clear();
    jet_phi.clear();
    jet_mass.clear();
    jet_et.clear();     
    jet_charge.clear();
    jet_mt.clear();
    jet_vx.clear();
    jet_vy.clear();
    jet_vz.clear();
    jet_nDaughters.clear();
    jet_vtxChi2.clear();
    jet_btagDeepCSV.clear();

    electron_nObjects = 0;
    electron_pt.clear();
    electron_eta.clear();
    electron_phi.clear();
    electron_mass.clear();
    electron_et.clear();     
    electron_charge.clear();

    muon_nObjects = 0;
    muon_pt.clear();
    muon_eta.clear();
    muon_phi.clear();
    muon_mass.clear();
    muon_et.clear();     
    muon_charge.clear();

    tau_nObjects = 0;
    tau_pt.clear();
    tau_eta.clear();
    tau_phi.clear();
    tau_mass.clear();
    tau_et.clear();     
    tau_charge.clear();

    MET_pt.clear();
    MET_eta.clear();
    MET_phi.clear();
}

DEFINE_FWK_MODULE(analysisObjectNtuplizer);
