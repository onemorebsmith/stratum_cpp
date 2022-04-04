#pragma once

#include <stratum.h>



namespace stratum_test{
    class test_miner{
        public:
            void start(const stratum::stratum_config& config);


        private:

        stratum::client_handle_t client;
    };
}