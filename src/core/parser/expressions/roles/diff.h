#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_DIFF_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_DIFF_H_

#include "../role.h"
#include "../../../elements/roles/diff.h"
#include "../../utils.h"

namespace dlplan::core::parser {

class DiffRole : public Role {
protected:
    element::Role_Ptr parse_role_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("DiffRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        element::Role_Ptr role_left = m_children[0]->parse_role(vocabulary, cache);
        element::Role_Ptr role_right = m_children[1]->parse_role(vocabulary, cache);
        if (!(role_left && role_right)) {
            throw std::runtime_error("DiffRole::parse_role_impl - children are not of type Role.");
        }
        // 2. Construct element
        return std::make_shared<element::DiffRole>(vocabulary, role_left, role_right);
    }

public:
    DiffRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}

#endif
