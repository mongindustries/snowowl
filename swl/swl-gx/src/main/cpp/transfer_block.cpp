#include "transfer_block.hpp"

SNOW_OWL_NAMESPACE(gx)

transfer_block::transfer_block  () noexcept = default;

transfer_block::~transfer_block () = default;


transfer_block::transfer_block            (transfer_block &&) noexcept = default;

transfer_block& transfer_block::operator= (transfer_block &&) noexcept = default;

SNOW_OWL_NAMESPACE_END
