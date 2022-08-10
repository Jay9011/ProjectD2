# ProjectD2
★ 실행 프로그램(exe) 파일은 압축 파일을 다운로드 받아 실행해주세요. ([ProjectD2.zip](https://github.com/Jay9011/ProjectD2/raw/master/ProjectD2.zip "ProjectD2.zip"))

 ## 목적
 처음 게임 포트폴리오를 계획 했을 때, 추억을 자극할 수 있는 어떤 게임을 만들고 싶었습니다.
 그러면서 동시에 Ori라는 게임을 인상 깊게 해서 벽을 타기도 하고, 무기를 바꿀 수도 있는 플랫포머 게임을 만들고 싶었습니다.
 
 ## 기능
 ### 1) 벽 타기
Ori라는 게임에서 가장 인상 깊었던 부분은 마치 애니메이션을 보는 듯한 부드러운 움직임과 연출, 그리고 뭔가 모르게 감동적인 스토리였습니다만, 이단 점프나 벽을 활용한 이동성으로 게임의 맛을 살린 부분도 있었다고 생각합니다.
그렇기에 무슨 일이 있어도 이단 점프와 벽 타기는 구현하고 싶었습니다.
처음에는 플레이어에게 주어진 하나의 충돌체만 가지고 움직이려 보니 버그가 많았습니다.
떨어지는 도중에 벽을 타고 싶은데 플랫폼 위에 올라간다든가, 위로 올라가면서 벽을 타고 싶은데 갑자기 위로 순간이동을 하거나 땅으로 꺼지는 버그였습니다.
이를 어떻게든 해결하고 싶어서 인터넷을 찾다 보니 좋은 아이디어를 얻게 되었습니다.
바로 손에 위치한 충돌체를 하나 더 만들어서 해당 충돌체와 충돌하면 벽을 타는 기능이었습니다.
간단한 아이디어였지만 미처 생각하지 못했던 부분이었고, 한 번 구현을 시작하고 보니 벽을 타는 문제뿐만 아니라 생각보다 많은 부분에서 좋은 기능을 하게 되었습니다.
손 충돌체를 길게 하면 몸 충돌체가 벽에 닿기 전이라 벽을 타기 전 가볍게 손을 가져다 대며 몸을 돌리는 애니메이션을 구현할 수 있었고, 충돌체에 대한 정보를 추가해 손에 닿은 충돌체가 벽을 탈 수 있는 충돌체인지 아닌지, 마찰력은 어떤지를 가져올 수 있었습니다.

 ### 2) SAP (Sweep and Prune)
 우선 플랫포머 게임을 만들기 전 충돌체에 대한 부담감을 줄이고 싶었습니다. 
 포트폴리오를 제작하기 전 WinAPI를 공부하면서 벽돌깨기 게임을 만들어 본 적 있습니다. 그때 충돌체 검사를 공 하나와 모든 벽돌에 대한 충돌 검사를 시행하면서 게임이 무거워지는 것이 느껴졌습니다.
 공부가 좀 더 필요하다고 생각했고, 그러면서 구매한 책에서 SAP라는 알고리즘을 알게 되었습니다. 축에 대해 충돌체들을 정렬시키고 분리 축이 있는 충돌체끼리는 충돌 검사를 시행하지 않고, 축이 겹치는 충돌체끼리만 충돌 검사를 시행한 후 다시 축 분리가 이루어진다면 충돌 검사를 끝내는 알고리즘이었습니다.
 마침 2D 플랫포머 게임이고 횡 스크롤 게임이다 보니 이걸 쓰면 좋겠다는 생각을 했습니다.
 바로 충돌체들을 전부 관리하는 충돌체 관리자를 만들어 씬에게 부여하고 씬에서 생성되는 모든 충돌체들을 생성시 해당 씬의 충돌체 관리자에게 알려주는 방법을 사용했습니다.
 충돌체 관리자는 마지막 업데이트시 가지고 있는 모든 충돌체들을 x축에 대해 정렬합니다. 

    void CollisionMgr::FinalUpdate()
    {
	    for (auto& collisionType : m_colliders)
	    {
		    std::sort(collisionType.begin(), collisionType.end(),
			    [](Collider* a, Collider* b)
			    {
				    return a->GetMin().x < b->GetMin().x;
			    }
		    );
	    }
    }

 ### 3) OBJECT_TYPE으로 충돌체 관리
이 부분은 SAP 알고리즘을 보기 전에 유튜브에서 공부하며 적용했던 부분입니다. OBJECT들에게 enum으로 Type을 구분시켜주고 이를 분리시켜두는 것을 보며 필요한 충돌체끼리만 충돌 검사를 할 수 있다는 생각이 들었습니다.
예를 들면 Player와 Platform만 검사하게 되면 Player의 위치 보정 검사가 끝나고, Player Bullet과 Monster만 검사하게 되면 Monster의 피격 검사를 끝낼 수 있는 방법이라고 생각했습니다.
코드는 간단했습니다.

vector<Collider*> m_colliders[(UINT)OBJECT_TYPE::FIN];

충돌체 벡터를 Type 개수에 맞춰 가지게 하고 충돌 검사시 OBJECT_TYPE으로 찾아서 검사할 수 있게 했습니다.

    bool CheckCollision(Collider* _chkCollider, OBJECT_TYPE _type, OUT vector<std::pair<Collider*, Collider*>>& _vecCollList);
    bool CheckCollision(OBJECT_TYPE _typeA, OBJECT_TYPE _typeB, OUT vector<std::pair<Collider*, Collider*>>& _vecCollList);

이때, 충돌 검사 이후 어떤 충돌체와 충돌했는지에 대한 정보도 필요할 것 같아 충돌 정보를 보내주는 OUT 벡터를 함께 만들었습니다.