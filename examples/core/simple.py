import dlplan


def construct_vocabulary_info():
    v = dlplan.VocabularyInfo()
    # Add predicates and constants of the domain.
    # Note that there are no constants in Blocksworld.
    v.add_predicate("on", 2)
    v.add_predicate("on_g", 2)
    v.add_predicate("ontable", 1)
    v.add_predicate("holding", 1)
    v.add_predicate("clear", 1)
    return v


def construct_instance_info(v):
    i = dlplan.InstanceInfo(v)
    # Add dynamic atoms
    i.add_atom("on", ["a", "b"])
    i.add_atom("on", ["b", "a"])
    i.add_atom("ontable", ["a"])
    i.add_atom("ontable", ["b"])
    i.add_atom("holding", ["a"])
    i.add_atom("holding", ["b"])
    i.add_atom("clear", ["a"])
    i.add_atom("clear", ["b"])
    # Add static goal atoms
    i.add_static_atom("on_g", ["a", "b"])
    # Add static atoms
    # Note that there are no static atoms in Blocksworld.
    return i


# The simplest example to illustrate the construction and evaluation of elements.
def main():
    # 1. Initialize VocabularyInfo
    v = construct_vocabulary_info()
    # 2. Initialize InstanceInfo
    i = construct_instance_info(v)
    # 3. Initialize SyntacticElementFactory
    f = dlplan.SyntacticElementFactory(v)

    # 4. Construct a state.
    atoms = i.get_atoms()
    a0 = atoms[0]
    a3 = atoms[3]
    a6 = atoms[6]
    state = dlplan.State(i, [a0, a3, a6])

    # 5. Parse and evaluate elements.
    numerical = f.parse_numerical("n_count(c_and(on_g(0),on(0)))")
    print(f"repr: {numerical.compute_repr()}")
    print(f"value: {numerical.evaluate(state)}")

    boolean = f.parse_boolean("b_empty(c_and(on_g(0),on(0)))")
    print(f"repr: {boolean.compute_repr()}")
    print(f"value: {boolean.evaluate(state)}")


if __name__ == "__main__":
    main()