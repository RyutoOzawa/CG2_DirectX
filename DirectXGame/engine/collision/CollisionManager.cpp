//当たり判定マネージャ
//コライダーのまとまりを持ち、全ての当たり判定をチェックする

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

	//すべての組み合わせについて総当たりチェック
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA) {
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB) {
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//属性で排斥
			if ((colA->attribute & colB->attribute) != 0) {
				//ANDで0でない(どこかしら属性が一致)なら判定しない
				continue;
			}


			//ともに球
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
				colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
				SphereCollider* spColA = dynamic_cast<SphereCollider*>(colA);
				SphereCollider* spColB = dynamic_cast<SphereCollider*>(colB);
				Sphere sphereA = static_cast<Sphere>(*spColA);
				Sphere sphereB = static_cast<Sphere>(*spColB);
				Vector3 inter;
				
				//colAの移動分
				Vector3 move = spColA->pos - spColA->GetOldPos();
				uint16_t count = (uint16_t)move.length();
				if (count >= 100) {
					ImGui::Text("count %d", count);
				}

				for (uint16_t i = 0; i < count; i++) {
					//球を少しずつ移動させて判定する
					sphereA.pos -= (move / count) * i;

					if (Collision::ColSphereToSphere(sphereA, sphereB, nullptr, &inter)) {
						colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
						colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
					}
				}

				//移動した球をリセット
				sphereA = static_cast<Sphere>(*spColA);
				move = spColB->pos - spColB->GetOldPos();
				count = (uint16_t)move.length();
				if (count >= 100) {
					ImGui::Text("count %d", count);
				}
				for (uint16_t i = 0; i < count; i++) {
					//球を少しずつ移動させて判定する
					sphereB.pos -= (move / count) * i;

					if (Collision::ColSphereToSphere(sphereA, sphereB, nullptr, &inter)) {
						colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
						colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
					}
				}

				//元の判定式
				/*if (Collision::ColSphereToSphere(*sphereA, *sphereB, nullptr, &inter)) {
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}*/

			}
		}
	}
}
