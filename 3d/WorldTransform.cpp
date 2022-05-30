#include "WorldTransform.h"
#include"Affine.h"

void WorldTransform::ParenChildUpdate()
{
	this->matWorld_ = WorldForm(*this);

	this->matWorld_ *= this->parent_->matWorld_;

	this->TransferMatrix();
}
