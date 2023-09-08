#pragma once

#include <ctime>

#include "../object/field.h"
#include "../object/camera.h"
#include "../object/dynamic_object.h"

namespace app {

    const clock_t                   MS_IN_SECOND        = 1000;
    const clock_t                   MS_MAX_SIMULATE     = 100;
    
    // Передвинуть объект.
    // В данной функции не учитывается случай, когда объект может переместиться более чем на одну клетку.
    void                            moveObject(object::IDynamicObjectPtr object, object::FieldPtr field, clock_t deltaTimeMs);

}
