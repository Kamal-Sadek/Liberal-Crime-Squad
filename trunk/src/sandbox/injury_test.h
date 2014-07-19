
#include "lcscommon.h"

enum bodyparts
{
   BODYPART_HEAD,
   BODYPART_BODY,
   BODYPART_ARM_RIGHT,
   BODYPART_ARM_LEFT,
   BODYPART_LEG_RIGHT,
   BODYPART_LEG_LEFT,
   BODYPARTNUM
};

#define WOUND_SHOT      BIT1
#define WOUND_CUT       BIT2
#define WOUND_BRUISED   BIT3
#define WOUND_BURNED    BIT4
#define WOUND_BLEEDING  BIT5
#define WOUND_TORN      BIT6
#define WOUND_NASTYOFF  BIT7
#define WOUND_CLEANOFF  BIT8

enum SpecialWounds
{
   SPECIALWOUND_TEETH,
   SPECIALWOUND_RIGHTEYE,
   SPECIALWOUND_LEFTEYE,
   SPECIALWOUND_NOSE,
   SPECIALWOUND_TONGUE,
   SPECIALWOUND_RIGHTLUNG,
   SPECIALWOUND_LEFTLUNG,
   SPECIALWOUND_HEART,
   SPECIALWOUND_LIVER,
   SPECIALWOUND_STOMACH,
   SPECIALWOUND_RIGHTKIDNEY,
   SPECIALWOUND_LEFTKIDNEY,
   SPECIALWOUND_SPLEEN,
   SPECIALWOUND_RIBS,
   SPECIALWOUND_NECK,
   SPECIALWOUND_UPPERSPINE,
   SPECIALWOUND_LOWERSPINE,
   SPECIALWOUNDNUM
};

#define RIBNUM 10
#define TOOTHNUM 32

class bodypart
{
private:
   char  type;
   short flag;
public:
   // Check if bodypart has sustained a specific injury
   bool CheckInjury(short wound)   { return flag & wound; }
   // Inflict an injury on the bodypart
   void InflictInjury(short wound) { flag |= wound; }
   // Heal an injury on the bodypart
   void HealInjury(short wound)    { flag &= wound; }

   // Check if bodypart is completely gone
   bool IsGone()   { return flag & (WOUND_NASTYOFF | WOUND_CLEANOFF); }
   // Check if any injury has been sustained
   bool IsInjured()   { return flag; }

   // Assign the bodypart type
   void SetType(char newtype) { type = newtype; }
};
