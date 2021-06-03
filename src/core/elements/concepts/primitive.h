#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"


namespace dlp {
namespace core {
namespace element {

class PrimitiveConcept : public Concept {
protected:
    const unsigned m_predicate_idx;
    const unsigned m_pos;

protected:
    virtual const ConceptDenotation& evaluate_impl(const StateImpl& state) override {
        const InstanceInfoImpl& info = *state.get_instance_info();
        // 2. Compute the result.
        m_result.clear();
        for (unsigned atom_idx : state.get_atom_idxs()) {
            const AtomImpl& atom = info.get_atom(atom_idx);
            if (atom.get_predicate_idx() == m_predicate_idx) {
                m_result.push_back(atom.get_object_idx(m_pos));
            }
        }
        return m_result;
    }

public:
    PrimitiveConcept(const VocabularyInfoImpl& vocabulary, const std::string& name, unsigned pos)
    : Concept(vocabulary, name), m_pos(pos), m_predicate_idx(vocabulary.get_predicate_idx(m_name)) {
        unsigned predicate_arity = vocabulary.get_predicate(m_predicate_idx).m_arity;
        if (m_pos >= predicate_arity) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate_arity) + ").");
        }
    }

    virtual unsigned compute_complexity() const override {
        return 1;
    }


    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos) << ")";
        return ss.str();
    }
};

}
}
}

#endif
