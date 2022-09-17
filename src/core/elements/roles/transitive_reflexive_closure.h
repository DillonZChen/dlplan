#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class TransitiveReflexiveClosureRole : public Role {
private:
    RoleDenotation compute_result(RoleDenotation&& denot, int num_objects) const {
        auto role_bitset = utils::role_denot_to_bitset(denot);
        for (int k = 0; k < num_objects; ++k) {
            for (int i = 0; i < num_objects; ++i) {
                for (int j = 0; j < num_objects; ++j) {
                    if (role_bitset.test(i * num_objects + k) && role_bitset.test(k * num_objects + j)) {
                        role_bitset.set(i * num_objects + j);
                    }
                }
            }
            role_bitset.set(k * num_objects + k);
        }
        return utils::bitset_to_role_denotation(role_bitset, num_objects);
    }

protected:
    const Role_Ptr m_role;

public:
    TransitiveReflexiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_num_objects();
        return compute_result(
            m_role->evaluate(state),
            num_objects);
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_role_denotation_cache.count(state, *this)) {
            return cache.m_role_denotation_cache.find(state, *this);
        }
        int num_objects = state.get_instance_info()->get_num_objects();
        auto result = compute_result(
            m_role->evaluate(state, cache),
            num_objects);
        cache.m_role_denotation_cache.insert(state, *this, result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_transitive_reflexive_closure";
    }
};

}

#endif
