#include "lalwe_appmanager.h"
#include "logger.h"
#include "constants.h"

LALWE_AppManager::LALWE_AppManager(QQmlContext* ctxt, QObject* obj) {
    this->ctxt = ctxt;
    this->guiObject = obj;
    assembler = new Assembler();
    processor.append(new Processor(*this));
    ram = processor.at(0)->getRam();
    programRunning = false;
    assemblyRunning = false;
    doAnimations = true;
    QObject::connect(&animationTicker,SIGNAL(timeout()), this, SIGNAL(stepAnimation()));
    QObject::connect(assembler, SIGNAL(assemblyDone()), this, SLOT(assemblyDone()));
    QObject::connect(obj,SIGNAL(toggleFollow(bool)),ram,SLOT(toggleFollowActive(bool)));
    logger = new Logger(*obj);
}

void LALWE_AppManager::assembleSlot(const QString &code) {
    if((!assemblyRunning || assembleHandle.isFinished()) && (!programRunning || programHandle.isFinished())) {
        setGuiProperty("status","Assembling...");
        assemblyRunning = true;
        assembleHandle = QtConcurrent::run(assembler, &Assembler::assemble, code.toStdString(), ram);
        animationTicker.start(Constants::ANIM_DELAY_MILIS);
    }
}

void LALWE_AppManager::saveProgramSlot(const QString &code, const QString &path, const int &mode) {
    std::ofstream saveFile(path.toStdString());
    if(saveFile.is_open()) {
        saveFile << code.toStdString() << std::endl;
        saveFile.close();
        guiObject->setProperty("fileName",path);
        QVariant arg = mode;
        QMetaObject::invokeMethod(guiObject, "continueWork",Q_ARG(QVariant,arg));
    } else {
        Logger::loggerInst->error("File save failed! Path: " + path.toStdString());
    }
}

void LALWE_AppManager::openProgramSlot(const QString &path) {
    std::ifstream openFile(path.toStdString());
    std::string code = "";
    std::string line = "";
    if(openFile.is_open()) {
        while(std::getline(openFile,line)) {
            code += line + "\n";
        }
        guiObject->setProperty("fileName",path);
        QVariant arg = QString::fromStdString(code);
        QMetaObject::invokeMethod(guiObject, "getCodeFromFile", Q_ARG(QVariant,arg));
    } else {
        Logger::loggerInst->error("File open failed! Path: " + path.toStdString());
    }
}

void LALWE_AppManager::playProgramSlot() {
    if((!programRunning || programHandle.isFinished())&&(!assemblyRunning || assembleHandle.isFinished())) {
        setGuiProperty("status","Simulation running...");
        Logger::loggerInst->info("Simulation started...");
        programRunning = true;
        programHandle = QtConcurrent::map(processor.begin(),processor.end(),[](Processor* p){ p->runProgram(); });
        animationTicker.start(Constants::ANIM_DELAY_MILIS);
    } else if(programRunning && programHandle.isRunning()){
        if(animationTicker.isActive()) {
            animationTicker.stop();
            setGuiProperty("status","Simulation paused");
        } else {
            animationTicker.start(Constants::ANIM_DELAY_MILIS);
            setGuiProperty("status","Simulation running...");
        }
    }
}

void LALWE_AppManager::toggleAnimationsSlot(const bool &newState) {
    processor.at(0)->toggleAnimations(newState);
}

void LALWE_AppManager::toggleRamHex(const bool &newState) {
    ram->toggleHexDisplay(newState);
}

void LALWE_AppManager::verifySlot(const QString &code) {
    assembler->verify(code.toStdString());
}

void LALWE_AppManager::setCtxProperty(const QString &name, const QVariant &data) {
    ctxt->setContextProperty(name,data);
}

void LALWE_AppManager::setGuiProperty(const QString &name, const QVariant &data) {
    guiObject->setProperty(name.toLatin1().data(), data);
}

void LALWE_AppManager::assemblyDone() {
    animationTicker.stop();
    setGuiProperty("status","Ready");
}

void LALWE_AppManager::animStepForward() {
    emit stepAnimation();
}

void LALWE_AppManager::windowClosing() {
    if((programRunning && programHandle.isRunning()) || (assemblyRunning && assembleHandle.isRunning())) {
        processor.at(0)->toggleAnimations(false);
    }
    emit stepAnimation();
}

void LALWE_AppManager::ramViewAddressChanged(const int &index, const QString &action) {
    ram->setRamViewAddress(index,action);
}

void LALWE_AppManager::printLine(const QString &line) {
    QMetaObject::invokeMethod(guiObject,"printLine",Q_ARG(QVariant,line));
}

void LALWE_AppManager::changeAnimSpeed(const double &percentage) {
    Constants::ANIM_DELAY_MILIS = Constants::ANIM_MAX_DELAY - (Constants::ANIM_MAX_DELAY - Constants::ANIM_MIN_DELAY) * percentage / 100;
    animationTicker.setInterval(Constants::ANIM_DELAY_MILIS);
}

void LALWE_AppManager::resetProcessor() {
    processor.at(0)->toggleAnimations(false);
    setGuiProperty("activeRamSlot1",-1);
    setGuiProperty("activeRamSlot2",-1);
    setGuiProperty("activeRegister",-1);
    setGuiProperty("aluActive",false);
    setGuiProperty("busToAluActive",false);
    setGuiProperty("busFromAluActive",false);
    setGuiProperty("cycleState",-1);
    setGuiProperty("operand",0);
    setGuiProperty("result",0);
    setGuiProperty("operation","+");
    setGuiProperty("decodedOpcode","N/A");
    setGuiProperty("effectiveAddress","N/A");
    setGuiProperty("addressMode","N/A");
    processor.at(0)->reset();
    processor.at(0)->toggleAnimations(doAnimations);
}

void LALWE_AppManager::getInput(const QString &input) {
    processor.at(0)->sendInput(input.toInt());
}
