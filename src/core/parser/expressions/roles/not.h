#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_NOT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_NOT_H_

#include "../role.h"
#include "../../../elements/roles/not.h"

namespace dlp {
namespace core {
namespace parser {

class NotRole : public Role {
protected:
    virtual element::Role_Ptr parse_role_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NotRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Role_Ptr role = m_children[0]->parse_role(vocabulary, cache);
        if (!role) {
            throw std::runtime_error("NotRole::parse_role_impl - child is not of type Role.");
        }
        // 2. Construct element
        return std::make_shared<element::NotRole>(vocabulary, role);
    }

public:
    NotRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}
}
}

#endif
