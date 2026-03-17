/*
*/

#ifndef _AUDIO_STATUS_H
#define _AUDIO_STATUS_H

#include <Arduino.h>

class AudioStatus
{
public:
    AudioStatus() { ClearCallbacks(); };
    virtual ~AudioStatus() { };

    void ClearCallbacks() { 
      metadata_fn_ = NULL; 
      status_fn_ = NULL; 
    };

    typedef void (*MetadataCallbackFn)(const char *tag, const char *type, const char *text, void *data);
    bool RegisterMetadataCallback(MetadataCallbackFn f, const char *tag, void *data) { 
      metadata_fn_ = f; 
      metadata_tag_ = tag; 
      metadata_data_ = data;
      return true; 
    }

    // Returns a unique warning/error code, varying by the object.  The string may be a PSTR, use _P functions!
    typedef void (*StatusCallbackFn)(const char *tag, int code, const char *text, void *data);
    bool RegisterStatusCallback(StatusCallbackFn f, const char *tag, void *data) { 
      status_fn_ = f; 
      status_tag_ = tag; 
      status_data_ = data;
      return true; 
    }

    // Safely call the md function, if defined
    inline void MetadataCB(const char *type, const char *text) { 
      if (metadata_fn_) metadata_fn_(metadata_tag_, type, text, metadata_data_); 
    }

    // Safely call the st function, if defined
    inline void StatusCB(int code, const char *text) { 
      if (status_fn_) status_fn_(status_tag_, code, text, status_data_); 
    }

private:
    MetadataCallbackFn metadata_fn_;
    const char *metadata_tag_;
    void *metadata_data_;

    StatusCallbackFn status_fn_;
    const char *status_tag_;
    void *status_data_;
};

#endif

