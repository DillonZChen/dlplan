#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"


namespace dlplan::core::element {

template<typename T>
class CountNumerical : public Numerical {
protected:
    const T m_element;

protected:
    const int& evaluate_impl(const State& state) override {
        m_result = m_element->evaluate(state).size();
        return m_result;
    }

public:
    CountNumerical(const VocabularyInfo& vocabulary, T element) : Numerical(vocabulary, "n_count"), m_element(element) { }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_element->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif