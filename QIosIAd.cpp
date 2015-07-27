
#include "QIosIAd.h"
#include "s3eIOSIAd.h"
#include "QLuaHelpers.h"
#include "lua.h"

using namespace quick;

namespace iosIAd {

//------------------------------------------------------------------------------
//C++ callbacks -> Lua events:

//iAd callbacks have no userdata, they just indicate an event_callback
//has happened

int32 BannerLoadedCallback(void* systemData, void* userData)
{
    LUA_EVENT_PREPARE("iosIAd");
    LUA_EVENT_SET_STRING("action", "bannerLoaded");
    LUA_EVENT_SEND();
    lua_pop(g_L, 1);

    return 0;
}

int32 FailedCallback(void* systemData, void* userData)
{
    LUA_EVENT_PREPARE("iosIAd");
    LUA_EVENT_SET_STRING("action", "failed");
    LUA_EVENT_SEND();
    lua_pop(g_L, 1);

    return 0;
}

int32 AdStartingCallback(void* systemData, void* userData)
{
    LUA_EVENT_PREPARE("iosIAd");
    LUA_EVENT_SET_STRING("action", "adStarting");
    LUA_EVENT_SEND();
    lua_pop(g_L, 1);

    return 0;
}

int32 AdFinishedCallback(void* systemData, void* userData)
{
    LUA_EVENT_PREPARE("iosIAd");
    LUA_EVENT_SET_STRING("action", "adFinished");
    LUA_EVENT_SEND();
    lua_pop(g_L, 1);

    return 0;
}

//------------------------------------------------------------------------------
//Functions:

bool isAvailable()
{
    return (bool)s3eIOSIAdAvailable();
}

// Error namespace/table members. These let you compare with member names
// (if iosIAd.getError() == errorDeviceFailed then...), but
// but print human readable names (print(iosIAd.getError()) -> "device failed")
const char* errorNone = "none";
const char* errorInvalidParameter = "invalid parameter";
const char* errorDeviceFailed = "device failed";
const char* errorUnsupportedAction = "unsupported action";
const char* errorWrongState = "wrong state";

// Using getError not getErrorString:
// - getErrorString often doesnt return user friendly strings in s3e APIs
// - getErrorString doesn't clear the state back to "none" after being called,
//   so could look like API is still in error state when it isn't.
const char* getError()
{
    s3eIOSIAdError err = s3eIOSIAdGetError();

    if (err == S3E_IOSIAD_ERROR_NONE)
        return errorNone;
    else if (err == S3E_IOSIAD_ERR_PARAM)
        return errorInvalidParameter;
    else if (err == S3E_IOSIAD_ERR_DEVICE)
        return errorDeviceFailed;
    else if (err == S3E_IOSIAD_ERR_DEVICE)
        return errorUnsupportedAction;
    else // err == S3E_IOSIAD_ERR_STATE
        return errorWrongState;
}

bool getPropEnum(const char* prop, s3eIOSIAdProperty* propEnum)
{   
    if (strcmp(prop, "running") == 0)
        *propEnum = S3E_IOSIAD_RUNNING;
    if (strcmp(prop, "loaded") == 0)
        *propEnum = S3E_IOSIAD_BANNER_LOADED;
    if (strcmp(prop, "show") == 0)
        *propEnum = S3E_IOSIAD_BANNER_SHOW;
    if (strcmp(prop, "width") == 0)
        *propEnum = S3E_IOSIAD_BANNER_WIDTH;
    if (strcmp(prop, "height") == 0)
        *propEnum = S3E_IOSIAD_BANNER_HEIGHT;
    else
        return false;
    
    return true;
}

int getProperty(const char* prop)
{
    s3eIOSIAdProperty propEnum;
    
    if (getPropEnum(prop, &propEnum) == false)
        return -1;
    
    return (int)s3eIOSIAdGetInt(propEnum);
}

bool setProperty(const char* prop, int value)
{
    s3eIOSIAdProperty propEnum;
    
    if (getPropEnum(prop, &propEnum) == false)
        return false;
    
    return s3eIOSIAdSetInt(propEnum, (int)value) == S3E_RESULT_SUCCESS ? true : false;
}

bool start()
{
    //Register all callbacks here:
    s3eIOSIAdRegister(S3E_IOSIAD_CALLBACK_BANNER_LOADED, BannerLoadedCallback, NULL);
    s3eIOSIAdRegister(S3E_IOSIAD_CALLBACK_FAILED, FailedCallback, NULL);
    s3eIOSIAdRegister(S3E_IOSIAD_CALLBACK_AD_STARTING, AdStartingCallback, NULL);
    s3eIOSIAdRegister(S3E_IOSIAD_CALLBACK_AD_FINISHED, AdFinishedCallback, NULL);

    //Calling start after callback setup
    if(s3eIOSIAdStart() == S3E_RESULT_ERROR)
    {
        stop();
        return false; //unregister callbacks
    }
    
    return true;
}

bool stop()
{
    //Un-register all callbacks here:
    s3eIOSIAdUnRegister(S3E_IOSIAD_CALLBACK_BANNER_LOADED, BannerLoadedCallback);
    s3eIOSIAdUnRegister(S3E_IOSIAD_CALLBACK_FAILED, FailedCallback);
    s3eIOSIAdUnRegister(S3E_IOSIAD_CALLBACK_AD_STARTING, AdStartingCallback);
    s3eIOSIAdUnRegister(S3E_IOSIAD_CALLBACK_AD_FINISHED, AdFinishedCallback);

    return s3eIOSIAdStop() == S3E_RESULT_SUCCESS ? true : false;
}

bool cancel()
{
    return s3eIOSIAdCancel() == S3E_RESULT_SUCCESS ? true : false;
}

bool s3eExtIOSIAdAvailable()
{
    return (bool)s3eExtIOSIAdAvailable();
}

} //namespace iosIAd
