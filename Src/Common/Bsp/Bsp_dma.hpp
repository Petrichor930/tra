#pragma once

#include "Soc.hpp"
#include HAL_INCLUDE

class Dma {
public:
    /**
     * @brief dma ram auto alloc 
     */
    void *ram_alloc(size_t size);

    /**
     * @brief dma ram alloc 
     */
    void *ram_alloc(size_t size, DmaRam_e _ram);


    /**
    * @brief dma get Instance
    */
    inline static Dma *inst() { return instance; }

private:
    static Dma *instance;
};
