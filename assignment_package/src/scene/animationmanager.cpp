#include "animationmanager.h"


AnimationManager::AnimationManager()
{
    Animation walk = Animation();
    walk.currFrame = 0;
    walk.isRecurring = true;
    walk.frameCount = 24;
    animation[0] = walk;

    Animation addRemoveBlock = Animation();
    addRemoveBlock.currFrame = 0;
    addRemoveBlock.isRecurring = false;
    addRemoveBlock.frameCount = 24;
    animation[1] = addRemoveBlock;

    Animation sayHi = Animation();
    sayHi.currFrame = 0;
    sayHi.isRecurring = false;
    sayHi.frameCount = 48;
    animation[2] = sayHi;

    Animation dab = Animation();
    dab.currFrame = 0;
    dab.isRecurring = false;
    dab.frameCount = 60;
    animation[3] = dab;

    Animation sonicRun = Animation();
    sonicRun.currFrame = 0;
    sonicRun.isRecurring = false;
    sonicRun.frameCount = 48;
    animation[4] = sonicRun;

    Animation tranquil = Animation();
    tranquil.currFrame = 0;
    tranquil.isRecurring = false;
    tranquil.frameCount = 120;
    animation[5] = tranquil;

    Animation thanks = Animation();
    thanks.currFrame = 0;
    thanks.isRecurring = false;
    thanks.frameCount = 60;
    animation[6] = thanks;

    currAnimation = -1;

    narutoRun.setSource(QUrl::fromLocalFile(":/sound/naruto.wav"));
    narutoRun.setLoopCount(1);
    narutoRun.setVolume(0.5f);

    hi.setSource(QUrl::fromLocalFile(":/sound/hi.wav"));
    hi.setLoopCount(1);
    hi.setVolume(0.5f);

    dabSound.setSource(QUrl::fromLocalFile(":/sound/dab.wav"));
    dabSound.setLoopCount(1);
    dabSound.setVolume(0.5f);

    meditation.setSource(QUrl::fromLocalFile(":/sound/meditation.wav"));
    meditation.setLoopCount(1);
    meditation.setVolume(1.f);

    clap.setSource(QUrl::fromLocalFile(":/sound/clap.wav"));
    clap.setLoopCount(1);
    clap.setVolume(0.5f);
}

AnimationManager::~AnimationManager(){}

void AnimationManager::play(int index)
{
    currAnimation = index;
    //animation[index].play();
    if (currAnimation == 4){
        narutoRun.play();
    }else if(currAnimation == 2){
        hi.play();
    }else if(currAnimation == 3){
        dabSound.play();
    }else if(currAnimation == 5){
        meditation.play();
    }else if(currAnimation == 6){
        clap.play();
    }
}

void AnimationManager::stop(int index)
{
    animation[index].currFrame = 0;
    //animation[index].stop();
    if(currAnimation == 4){
        narutoRun.stop();
    }else if(currAnimation == 2){
        hi.stop();
    }else if(currAnimation == 3){
        dabSound.stop();
    }else if(currAnimation == 5){
        meditation.stop();
    }else if(currAnimation == 6){
        clap.stop();
    }

    if (currAnimation == index){
        currAnimation = -1;
    }
}

//bool AnimationManager::isPlaying(int index){
//    return animation[index].isRecurring;
//}

void AnimationManager::tick()
{
    if (currAnimation != -1){
        if (animation[currAnimation].currFrame == animation[currAnimation].frameCount){
            animation[currAnimation].currFrame = 0;
            if (!animation[currAnimation].isRecurring){
                stop(currAnimation);
            }
        }else{
            animation[currAnimation].currFrame += 1;
        }
    }
}

int AnimationManager::getCurrAnimation() const
{
    return currAnimation;
}

int AnimationManager::getCurrFrame() const
{
    if (currAnimation != -1) return animation[currAnimation].currFrame;
    return -1;
}
