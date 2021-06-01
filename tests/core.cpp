
#include <gtest/gtest.h>
#include "dlp/core.h"

// Demonstrate some basic assertions.
TEST(DLPTests, InstanceCreation) {
    // Add predicates
    dlp::core::VocabularyInfo vocabulary;
    dlp::core::Predicate p0 = vocabulary.add_predicate("on", 2);
    dlp::core::Predicate p1 = vocabulary.add_predicate("onTable", 1);
    dlp::core::Predicate p2 = vocabulary.add_predicate("holding", 1);
    dlp::core::Predicate p3 = vocabulary.add_predicate("on_g", 2);
    std::vector<dlp::core::Predicate> predicates({p0, p1, p2, p3});
    dlp::core::InstanceInfo instance(vocabulary);
    // Add state atoms
    dlp::core::Atom a0 = instance.add_atom("on", {"A", "B"});
    dlp::core::Atom a1 = instance.add_atom("on", {"B", "A"});
    dlp::core::Atom a2 = instance.add_atom("onTable", {"A"});
    dlp::core::Atom a3 = instance.add_atom("onTable", {"B"});
    dlp::core::Atom a4 = instance.add_atom("holding", {"A"});
    dlp::core::Atom a5 = instance.add_atom("holding", {"B"});
    // Add goal atoms
    dlp::core::Atom a6 = instance.add_static_atom("on_g", {"A", "B"});

    dlp::core::SyntacticElementFactory factory(vocabulary);

    dlp::core::Numerical numerical = factory.parse_numerical("n_count(c_and(on_g(0),on(0)))");
    EXPECT_EQ(numerical.compute_complexity(), 4);

    dlp::core::State state = instance.convert_state({a0, a1});
    EXPECT_EQ(numerical.evaluate(state), 1);
}

TEST(DLPTests, CreateSConcept) {
    dlp::core::VocabularyInfo vocabulary;
    dlp::core::Predicate p0 = vocabulary.add_predicate("clear", 2);
    dlp::core::SyntacticElementFactory factory(vocabulary);

    auto numerical = factory.parse_numerical("n_count(clear)");
    EXPECT_EQ(numerical.compute_complexity(), 2);
}