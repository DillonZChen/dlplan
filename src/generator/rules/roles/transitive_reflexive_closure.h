#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"

namespace dlplan::generator::rules {

class TransitiveReflexiveClosureRole : public Role {
public:
    TransitiveReflexiveClosureRole() : Role("r_transitive_reflexive_closure") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 1) {
            core::SyntacticElementFactory& factory = data.m_factory;
            for (const auto& r : data.m_roles_by_iteration[iteration]) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_transitive_reflexive_closure(r))));
            }
        }
    }
};

}

#endif
