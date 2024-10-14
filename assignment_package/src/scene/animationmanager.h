#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H
#include <map>
#include <QSoundEffect>

//class Animation{
//private:
//    bool isPlaying;
//    int currFrame;
//    int frameCount;
//public:
//    Animation();
//    ~Animation();
//    void play();
//    void stop();

//    friend class AnimationManager;
//};
struct Animation{
    bool isRecurring;
    int currFrame;
    int frameCount;
};

class AnimationManager{
private:
    std::array<Animation,7> animation;
    QSoundEffect hi;
    QSoundEffect narutoRun;
    QSoundEffect dabSound;
    QSoundEffect meditation;
    QSoundEffect clap;
public:
    int currAnimation;
    AnimationManager();
    ~AnimationManager();
    void play(int index);
    void stop(int index);
    void tick();

    int getCurrAnimation() const;
    int getCurrFrame() const;
};

#endif // ANIMATIONMANAGER_H
