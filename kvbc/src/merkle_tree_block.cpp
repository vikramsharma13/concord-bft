#include "merkle_tree_block.h"

#include "assertUtils.hpp"
#include "direct_kv_block.h"
#include "kv_types.hpp"
#include "merkle_tree_serialization.h"

#include <cstdint>

namespace concord::kvbc::v2MerkleTree::block {

using sparse_merkle::Hash;
using ::concordUtils::Sliver;

// Use the v1DirectKeyValue implementation and just add the state hash at the back. We want that so it is included in
// the block digest. We can do that, because users are not expected to interpret the returned buffer themselves.
RawBlock create(const SetOfKeyValuePairs &updates, const void *parentDigest, const Hash &stateHash) {
  SetOfKeyValuePairs out;
  return v1DirectKeyValue::block::create(
      updates, out, parentDigest, stateHash.dataArray().data(), stateHash.dataArray().size());
}

SetOfKeyValuePairs getData(const RawBlock &block) { return v1DirectKeyValue::block::getData(block); }

const void *getParentDigest(const RawBlock &block) { return v1DirectKeyValue::block::getParentDigest(block); }

Hash getStateHash(const RawBlock &block) {
  Assert(block.length() >= Hash::SIZE_IN_BYTES);
  const auto data = reinterpret_cast<const std::uint8_t *>(block.data());
  return Hash{data + (block.length() - Hash::SIZE_IN_BYTES)};
}

namespace detail {

Sliver createNode(const Node &node) { return v2MerkleTree::detail::serialize(node); }

Node parseNode(const Sliver &buffer) { return v2MerkleTree::detail::deserialize<Node>(buffer); }
}  // namespace detail

}  // namespace concord::kvbc::v2MerkleTree::block
