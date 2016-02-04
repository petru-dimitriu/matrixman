#pragma once

namespace MatrixMan
{
    class MMError
    {
        private:
            char* message;
        public:
            MMError();
            MMError(char* message);
            ~MMError();
            void display();

    };
}
