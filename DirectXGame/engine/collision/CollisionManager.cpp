//�����蔻��}�l�[�W��
//�R���C�_�[�̂܂Ƃ܂�������A�S�Ă̓����蔻����`�F�b�N����

#include "CollisionManager.h"
#include"BaseCollider.h"
#include"Collision.h"
#include"ImguiManager.h"
#include"SphereCollider.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;

	return &instance;
}

void CollisionManager::CheckAllCollisions()
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	ImGui::Text("collider %d", std::distance(colliders.begin(), colliders.end()));

	//���ׂĂ̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//�����Ŕr��
			if ((colA->attribute & colB->attribute) != 0) {
				//AND��0�łȂ�(�ǂ������瑮������v)�Ȃ画�肵�Ȃ�
				continue;
			}


			//�Ƃ��ɋ�
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				SphereCollider* spColA = dynamic_cast<SphereCollider*>(colA);
				SphereCollider* spColB = dynamic_cast<SphereCollider*>(colB);
				Sphere sphereA = static_cast<Sphere>(*spColA);
				Sphere sphereB = static_cast<Sphere>(*spColB);
				Vector3 inter;
				
				//colA�̈ړ���
				Vector3 move = spColA->pos - spColA->GetOldPos();
				uint16_t count = (uint16_t)move.length();
				if (count >= 100) {
					ImGui::Text("count %d", count);
				}

				for (uint16_t i = 0; i < count; i++) {
					//�����������ړ������Ĕ��肷��
					sphereA.pos -= (move / count) * i;

					if (Collision::ColSphereToSphere(sphereA, sphereB, nullptr, &inter)) {
						colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
						colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
					}
				}

				//�ړ������������Z�b�g
				sphereA = static_cast<Sphere>(*spColA);
				move = spColB->pos - spColB->GetOldPos();
				count = (uint16_t)move.length();
				if (count >= 100) {
					ImGui::Text("count %d", count);
				}
				for (uint16_t i = 0; i < count; i++) {
					//�����������ړ������Ĕ��肷��
					sphereB.pos -= (move / count) * i;

					if (Collision::ColSphereToSphere(sphereA, sphereB, nullptr, &inter)) {
						colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
						colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
					}
				}

				//���̔��莮
				/*if (Collision::ColSphereToSphere(*sphereA, *sphereB, nullptr, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}*/

			}
		}
	}
}
