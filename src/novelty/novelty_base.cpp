#include "../../include/dlplan/novelty.h"

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "../utils/math.h"


namespace dlplan::novelty {

NoveltyBase::NoveltyBase(std::vector<int>&& factors, int num_atoms, int arity)
    : m_factors(std::move(factors)), m_num_atoms(num_atoms), m_arity(arity) { }

NoveltyBase::NoveltyBase(int num_atoms, int arity)
    : m_num_atoms(num_atoms), m_arity(arity) {
    if (m_arity < 0) {
        throw std::runtime_error("NoveltyBase::NoveltyBase - arity must be greater than or equal to 0.");
    }
    m_factors = std::vector<int>(m_arity);
    for (int i = 0; i < m_arity; ++i) {
        m_factors[i] = std::pow(m_num_atoms+1, i);
    }
}

NoveltyBase::NoveltyBase(const NoveltyBase& other) = default;

NoveltyBase& NoveltyBase::operator=(const NoveltyBase& other) = default;

NoveltyBase::NoveltyBase(NoveltyBase&& other) = default;

NoveltyBase& NoveltyBase::operator=(NoveltyBase&& other) = default;

NoveltyBase::~NoveltyBase() = default;

TupleIndex NoveltyBase::atom_indices_to_tuple_index(const AtomIndices& atom_indices) const {
    assert(static_cast<int>(atom_indices.size()) == m_arity);
    assert(std::is_sorted(atom_indices.begin(), atom_indices.end()));
    TupleIndex result = 0;
    int i = 0;
    for (auto atom_index : atom_indices) {
        result += m_factors[i] * (atom_index + 1);
        ++i;
    }
    return result;
}

AtomIndices NoveltyBase::tuple_index_to_atom_indices(TupleIndex tuple_index) const {
    AtomIndices result;
    for (int i = m_arity-1; i >= 0; --i) {
        int atom_index = tuple_index / m_factors[i];
        if (atom_index != 0) {
            result.push_back(atom_index - 1);
        }
        tuple_index -= atom_index * m_factors[i];
    }
    std::reverse(result.begin(), result.end());
    return result;
}

const std::vector<int>& NoveltyBase::get_factors() const {
    return m_factors;
}

int NoveltyBase::get_num_atoms() const {
    return m_num_atoms;
}

int NoveltyBase::get_arity() const {
    return m_arity;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::novelty::NoveltyBase& /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::novelty::NoveltyBase* t, const unsigned int /* version */ )
{
    ar << t->m_factors;
    ar << t->m_num_atoms;
    ar << t->m_arity;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::novelty::NoveltyBase* t, const unsigned int /* version */ )
{
    std::vector<int> factors;
    int num_atoms;
    int arity;
    ar >> factors;
    ar >> num_atoms;
    ar >> arity;
    ::new(t)dlplan::novelty::NoveltyBase(std::move(factors), num_atoms, arity);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::novelty::NoveltyBase& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::novelty::NoveltyBase& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::novelty::NoveltyBase* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::novelty::NoveltyBase* t, const unsigned int version);

}