#include "policy_impl.h"

#include <sstream>

#include "condition.h"
#include "effect.h"

#include "../include/dlplan/core.h"
#include "../include/dlplan/policy.h"


namespace dlplan::policy {

PolicyImpl::PolicyImpl(
    std::shared_ptr<const PolicyRoot> root,
    std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
    std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
    std::vector<std::shared_ptr<const Rule>>&& rules)
    : m_root(root),
      m_boolean_features(std::move(boolean_features)),
      m_numerical_features(std::move(numerical_features)),
      m_rules(std::move(rules)),
      m_evaluation_caches(std::make_shared<EvaluationCaches>(m_boolean_features.size(), m_numerical_features.size())) { }

std::pair<std::shared_ptr<const Rule>, bool> PolicyImpl::evaluate(int source_index, const core::State& source, int target_index, const core::State& target) {
    if (source_index < 0 || target_index < 0) {
        throw std::runtime_error("PolicyImpl::evaluate: source or target index cannot be negative.");
    }
    for (auto& r : m_rules) {
        if (r->evaluate_conditions(source_index, source, *m_evaluation_caches) && r->evaluate_effects(source_index, source, target_index, target, *m_evaluation_caches))
            return std::make_pair(r, true);
    }
    return std::make_pair(nullptr, false);
}

std::string PolicyImpl::compute_repr() const {
    std::stringstream ss;
    ss << "(:policy\n";
    // boolean features
    ss << "(:boolean_features ";
    for (const auto& b : m_boolean_features) {
        ss << "\"" << b->compute_repr() << "\"";
        if (b != m_boolean_features.back()) ss << " ";
    }
    ss << ")\n";
    // numerical_features
    ss << "(:numerical_features ";
    for (const auto& n : m_numerical_features) {
        ss << "\"" << n->compute_repr() << "\"";
        if (n != m_numerical_features.back()) ss << " ";
    }
    ss << ")\n";
    // rules
    for (const auto& r : m_rules) {
        ss << r->compute_repr() << "\n";
    }
    ss << ")";
    return ss.str();
}

std::shared_ptr<const PolicyRoot> PolicyImpl::get_root() const {
    return m_root;
}

std::vector<std::shared_ptr<const BooleanFeature>> PolicyImpl::get_boolean_features() const {
    return m_boolean_features;
}

std::vector<std::shared_ptr<const NumericalFeature>> PolicyImpl::get_numerical_features() const {
    return m_numerical_features;
}

void PolicyImpl::clear_evaluation_cache() {
    m_evaluation_caches = std::make_shared<EvaluationCaches>(m_boolean_features.size(), m_numerical_features.size());
}

}