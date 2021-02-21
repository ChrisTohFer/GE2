#pragma once

namespace plat {

    class Window
    {
    public:

    private:
        //Pointer to implementation to avoid unnecessary inclusions in header
        class Impl;
        Impl* m_impl;

        //Data from window impl
        //size
        //input
        //
    };

}