/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/ // :3
#ifndef __CCPARTICLE_EXAMPLE_H__ // :3
#define __CCPARTICLE_EXAMPLE_H__ // :3

#include "CCParticleSystemQuad.h" // :3

NS_CC_BEGIN // :3

/**
 * @addtogroup particle_nodes
 * @{
 */ // :3

//! @brief A fire particle system :3
class CC_DLL CCParticleFire : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleFire(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleFire(){} // :3
    bool init(){ return initWithTotalParticles(250); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleFire* create(); // :3
    static CCParticleFire* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief A fireworks particle system :3
class CC_DLL CCParticleFireworks : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleFireworks(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleFireworks(){} // :3
    bool init(){ return initWithTotalParticles(1500); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleFireworks* create(); // :3
    static CCParticleFireworks* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief A sun particle system :3
class CC_DLL CCParticleSun : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleSun(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleSun(){} // :3
    bool init(){ return initWithTotalParticles(350); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleSun* create(); // :3
    static CCParticleSun* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief A galaxy particle system :3
class CC_DLL CCParticleGalaxy : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleGalaxy(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleGalaxy(){} // :3
    bool init(){ return initWithTotalParticles(200); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleGalaxy* create(); // :3
    static CCParticleGalaxy* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief A flower particle system :3
class CC_DLL CCParticleFlower : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleFlower(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleFlower(){} // :3
    bool init(){ return initWithTotalParticles(250); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleFlower* create(); // :3
    static CCParticleFlower* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief A meteor particle system :3
class CC_DLL CCParticleMeteor : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleMeteor(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleMeteor(){} // :3
    bool init(){ return initWithTotalParticles(150); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleMeteor * create(); // :3
    static CCParticleMeteor* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief An spiral particle system :3
class CC_DLL CCParticleSpiral : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleSpiral(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleSpiral(){} // :3
    bool init(){ return initWithTotalParticles(500); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleSpiral* create(); // :3
    static CCParticleSpiral* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief An explosion particle system :3
class CC_DLL CCParticleExplosion : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleExplosion(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleExplosion(){} // :3
    bool init(){ return initWithTotalParticles(700); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleExplosion* create(); // :3
    static CCParticleExplosion* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief An smoke particle system :3
class CC_DLL CCParticleSmoke : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleSmoke(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleSmoke(){} // :3
    bool init(){ return initWithTotalParticles(200); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleSmoke* create(); // :3
    static CCParticleSmoke* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief An snow particle system :3
class CC_DLL CCParticleSnow : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleSnow(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleSnow(){} // :3
    bool init(){ return initWithTotalParticles(700); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleSnow* create(); // :3
    static CCParticleSnow* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

//! @brief A rain particle system :3
class CC_DLL CCParticleRain : public CCParticleSystemQuad // :3
{ // :3
    GEODE_FRIEND_MODIFY // :3
public: // :3
    /**
     * @js ctor
     */ // :3
    CCParticleRain(){} // :3
    /**
     * @js NA
     * @lua NA
     */ // :3
    virtual ~CCParticleRain(){} // :3
    bool init(){ return initWithTotalParticles(1000); } // :3
    virtual bool initWithTotalParticles(unsigned int numberOfParticles); // :3

    static CCParticleRain* create(); // :3
    static CCParticleRain* createWithTotalParticles(unsigned int numberOfParticles); // :3
}; // :3

// end of particle_nodes group :3
/// @} :3

NS_CC_END // :3

#endif //__CCPARTICLE_EXAMPLE_H__ :3
