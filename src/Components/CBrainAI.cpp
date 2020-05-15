#include "CBrainAI.h"

CBrainAI::CBrainAI(){
    m_compType = CompType::BrainAIComp;

    //fuzzy logic del coche para el movimiento (giro y aceleracion)
    fuzzyLogic = make_shared<SystemFuzzyLogicAI>();
}

CBrainAI::CBrainAI(BrainAIDifficult dif) : CBrainAI() {
    difficult = dif;
}
 


void CBrainAI::CleanVisionRange(){
    carInVision.clear();
    boxInVision.clear();
    totemInVision = nullptr;
}