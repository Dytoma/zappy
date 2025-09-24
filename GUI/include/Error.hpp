/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Error
*/

#ifndef ERROR_HPP_
    #define ERROR_HPP_
    #include <exception>
    #include <string>

namespace Error {

    class invalid_argument : std::exception {
        public:
            invalid_argument(const std::string &msg);

            const char *what() const noexcept override;
    
        private:
            std::string message;
    };

    class runtime_error : std::exception {
        public:
            runtime_error(const std::string &msg);

            const char *what() const noexcept override;
        
        private:
            std::string message;
    };
}

void check_arguments(int, char **);

#endif /* !ERROR_HPP_ */
