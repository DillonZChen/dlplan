#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../concept.h"


namespace dlplan::core::element {

class AndConcept : public Concept {
protected:
    const Concept_Ptr m_concept_left;
    const Concept_Ptr m_concept_right;

protected:
    const ConceptDenotation& evaluate_impl(const State& state) override {
        const ConceptDenotation& l_vec = m_concept_left->evaluate(state);
        const ConceptDenotation& r_vec = m_concept_right->evaluate(state);
        ConceptDenotation_Set r_set(r_vec.begin(), r_vec.end());
        m_result.clear();
        for (int c : l_vec) {
            if (r_set.find(c) != r_set.end()) {
                m_result.push_back(c);
            }
        }
        return m_result;
    }

public:
    AndConcept(const VocabularyInfo& vocabulary, Concept_Ptr concept_1, Concept_Ptr concept_2)
    : Concept(vocabulary, "c_and"),
      m_concept_left(concept_1->compute_repr() < concept_2->compute_repr() ? concept_1 : concept_2),
      m_concept_right(concept_1->compute_repr() < concept_2->compute_repr() ? concept_2 : concept_1) {
        if (!(concept_1 && concept_2)) {
            throw std::runtime_error("AndConcept::AndConcept - at least one child is a nullptr.");
        }
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept_left->compute_repr() << "," << m_concept_right->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
