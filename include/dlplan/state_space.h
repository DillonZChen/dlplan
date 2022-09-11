#ifndef DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_
#define DLPLAN_INCLUDE_DLPLAN_STATE_SPACE_H_

#include "core.h"


namespace dlplan::state_space {

using StateIndex = int;
using StateIndices = std::vector<StateIndex>;
using StateIndicesSet = std::unordered_set<StateIndex>;
using StateIndexPairs = std::vector<std::pair<StateIndex, StateIndex>>;
using AdjacencyList = std::vector<StateIndices>;
using Distance = int;
using Distances = std::vector<int>;

const int INF = std::numeric_limits<int>::max();
const int UNDEFINED = -1;


class StateSpace {
private:
    /* Required information. */
    std::shared_ptr<const core::InstanceInfo> m_instance_info;
    core::States m_states;
    StateIndex m_initial_state_index;
    StateIndices m_forward_successor_state_indices;
    StateIndices m_forward_successor_state_indices_offsets;
    StateIndicesSet m_goal_state_indices;
    /* Derived information that we precompute in the constructor. */
    StateIndices m_backward_successor_state_indices;
    StateIndices m_backward_successor_state_indices_offsets;
    StateIndicesSet m_deadend_state_indices;
    Distances m_goal_distances;

private:
    /**
     * Initializes derived information.
     */
    void initialize();

public:
    StateSpace(
        std::shared_ptr<const core::InstanceInfo>&& instance_info,
        core::States&& states,
        StateIndex initial_state_index,
        AdjacencyList&& adjacency_matrix,
        StateIndicesSet&& goal_state_indices);
    StateSpace(const StateSpace& other);
    StateSpace& operator=(const StateSpace& other);
    StateSpace(StateSpace&& other);
    StateSpace& operator=(StateSpace&& other);
    ~StateSpace();

    /**
     * Prunes the state space and returns the mapping from new to old state indices.
     */
    StateIndices prune_states(const StateIndicesSet& state_indices);

    /**
     * Run backward BrFs to compute distances.
     */
    Distances compute_distances_to_states(const StateIndicesSet& state_indices) const;

    /**
     * For more readable iterations.
     */
    void for_each_state_index(std::function<void(int state_index)>&& function) const;
    void for_each_forward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const;
    void for_each_backward_successor_state_index(std::function<void(int state_index)>&& function, int state_index) const;

    /**
     * Convenience functions.
     */
    bool is_goal(StateIndex state_index) const;
    bool is_nongoal(StateIndex state_index) const;
    bool is_deadend(StateIndex state_index) const;
    bool is_alive(StateIndex state_index) const;

    /**
     * Getters.
     */
    const core::States& get_states_ref() const;
    const core::State& get_state_ref(int index) const;
    int get_num_states() const;
    const Distances& get_goal_distances_ref() const;
    std::shared_ptr<const core::InstanceInfo> get_instance_info() const;
};


class StateSpaceGenerator {
public:
    /**
     * Generates the StateSpace containing the reachable states
     * from given PDDL domain and instance files.
     */
    StateSpace generate_state_space(
        const std::string& domain_file,
        const std::string& instance_file) const;
};

}

#endif
