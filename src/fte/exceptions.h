#ifndef _LIBFTE_SRC_EXCEPTIONS_H
#define _LIBFTE_SRC_EXCEPTIONS_H

#include <exception>

namespace fte {

// Exceptions

class FteException : public std::exception {
    virtual const char* what() const throw() {
        return "FteException";
    }
};


class InvalidRankInput : public FteException {
    virtual const char* what() const throw() {
        return "Invalid rank input: ensure integer is within the correct range.";
    }
};

class InvalidUnrankInput : public FteException {
    virtual const char* what() const throw() {
        return "Invalid unrank input: ensure string is exactly fixed length sizee.";
    }
};

class InvalidFstFormat : public FteException {
    virtual const char* what() const throw() {
        return "Invalid FST format.";
    }
};

class InvalidFstStateName : public FteException {
    virtual const char* what() const throw() {
        return "Invalid ranker format: ranker has N states, and a state that is not in the range 0,1,...,N-1.";
    }
};

class InvalidSymbolRange : public FteException {
    virtual const char* what() const throw() {
        return "Invalid ranker format: ranker has symbol that is not in the range 0,1,...,255.";
    }
};


class InvalidInputNoAcceptingPaths : public FteException {
    virtual const char* what() const throw() {
        return "Please verify your input, the string does not result in an accepting path in the ranker.";
    }
};

class InvalidSymbol : public FteException {
    virtual const char* what() const throw() {
        return "Please verify your input, it contains a symbol not in the sigma of the ranker.";
    }
};

} // namespace fte

#endif /* _LIBFTE_SRC_EXCEPTIONS_H */
