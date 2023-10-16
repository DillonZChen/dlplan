#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_ELEMENTS_COMMON_ERROR_HANDLER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_ELEMENTS_COMMON_ERROR_HANDLER_HPP_

#include <map>

#include "include/dlplan/core/parsers/common/config.hpp"


namespace dlplan::policy::parsers::elements
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    //  Our error handler
    ////////////////////////////////////////////////////////////////////////////

    struct error_handler_base
    {
        error_handler_base();

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& first, Iterator const& last
          , Exception const& x, Context const& context);

        std::map<std::string, std::string> id_map;
    };

    ////////////////////////////////////////////////////////////////////////////
    // Implementation
    ////////////////////////////////////////////////////////////////////////////

    inline error_handler_base::error_handler_base()
    {
        // TODO: add more precise error messages.
        id_map["name"] = "name";
        id_map["constant"] = "constant";
        id_map["predicate"] = "predicate";
        id_map["position"] = "position";
        id_map["boolean_inner"] = "boolean_inner";
        id_map["concept_inner"] = "concept_inner";
        id_map["numerical_inner"] = "numerical_inner";
        id_map["role_inner"] = "role_inner";
        id_map["element_inner"] = "element_inner";
        id_map["concept_or_role_inner"] = "concept_or_role_inner";
        id_map["concept_or_role"] = "concept_or_role";
        id_map["empty_boolean"] = "empty_boolean";
        id_map["inclusion_boolean"] = "inclusion_boolean";
        id_map["nullary_boolean"] = "nullary_boolean";
        id_map["all_concept"] = "all_concept";
        id_map["and_concept"] = "and_concept";
        id_map["bot_concept"] = "bot_concept";
        id_map["diff_concept"] = "diff_concept";
        id_map["equal_concept"] = "equal_concept";
        id_map["not_concept"] = "not_concept";
        id_map["one_of_concept"] = "one_of_concept";
        id_map["or_concept"] = "or_concept";
        id_map["primitive_concept"] = "primitive_concept";
        id_map["projection_concept"] = "projection_concept";
        id_map["some_concept"] = "some_concept";
        id_map["subset_concept"] = "subset_concept";
        id_map["top_concept"] = "top_concept";
        id_map["concept_distance_numerical"] = "concept_distance_numerical";
        id_map["count_numerical"] = "count_numerical";
        id_map["role_distance_numerical"] = "role_distance_numerical";
        id_map["sum_concept_distance_numerical"] = "sum_concept_distance_numerical";
        id_map["sum_role_distance_numerical"] = "sum_role_distance_numerical";
        id_map["and_role"] = "and_role";
        id_map["compose_role"] = "compose_role";
        id_map["diff_role"] = "diff_role";
        id_map["identity_role"] = "identity_role";
        id_map["inverse_role"] = "inverse_role";
        id_map["not_role"] = "not_role";
        id_map["or_role"] = "or_role";
        id_map["primitive_role"] = "primitive_role";
        id_map["restrict_role"] = "restrict_role";
        id_map["top_role"] = "top_role";
        id_map["transitive_closure_role"] = "transitive_closure_role";
        id_map["transitive_reflexive_closure_role"] = "transitive_reflexive_closure_role";
        id_map["boolean"] = "boolean";
        id_map["numerical"] = "numerical";
        id_map["concept"] = "concept";
        id_map["role"] = "role";
        id_map["element"] = "element";
    }

    template <typename Iterator, typename Exception, typename Context>
    inline x3::error_handler_result
    error_handler_base::on_error(
        Iterator&, Iterator const&
      , Exception const& x, Context const& context)
    {
        std::string which = x.which();
        auto iter = id_map.find(which);
        if (iter != id_map.end())
            which = iter->second;

        std::string message = "Error! Expecting: " + which + " here:";
        auto& error_handler = x3::get<error_handler_tag>(context).get();
        error_handler(x.where(), message);
        return x3::error_handler_result::fail;
    }
}

#endif