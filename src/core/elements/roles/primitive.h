#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class PrimitiveRole : public Role {
protected:
    const unsigned m_predicate_idx;
    const unsigned m_pos_1;
    const unsigned m_pos_2;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const InstanceInfoImpl& info = *state.get_instance_info();
        // 2. Compute the result.
        m_result.clear();
        for (unsigned atom_idx : state.get_atom_idxs()) {
            const AtomImpl& atom = info.get_atom(atom_idx);
            if (atom.get_predicate_idx() == m_predicate_idx) {
                m_result.emplace_back(atom.get_object_idx(m_pos_1), atom.get_object_idx(m_pos_2));
            }
        }
        return m_result;
    }

public:
    PrimitiveRole(const VocabularyInfoImpl& vocabulary, const std::string& name, unsigned pos_1, unsigned pos_2)
    : Role(vocabulary, name), m_predicate_idx(vocabulary.get_predicate_idx(name)), m_pos_1(pos_1), m_pos_2(pos_2) {
        unsigned predicate_arity = vocabulary.get_predicate(m_predicate_idx).m_arity;
        if (m_pos_1 >= predicate_arity || m_pos_2 >= predicate_arity) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate_arity) + ").");
        }
    }

    virtual unsigned compute_complexity() const override {
        return 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
        return ss.str();
    }
};

}
}
}

#endif
