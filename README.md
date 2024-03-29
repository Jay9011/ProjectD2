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

### 4) 충돌 정보 처리 (Enter, Stay, Exit)
충돌체 관리를 위와 같이 하다 보니 하나의 문제가 생겼습니다.  
Event 처리를 하고 싶은데 충돌체 검사가 매 프레임마다 이루어지다 보니 Stay의 검사만 계속하고 있던 것입니다. 충돌 시작시 딱 한 번 Event를 발생시키고 싶었습니다.  

찾아보니 유니티에 Enter, Stay, Exit의 처리가 있었고, 해당 기능을 만들고 싶었습니다.  
이미 충돌체 충돌 검사 구조는 구현이 되어있고 현재 구현된 기능 속에서 최대한 비슷한 기능을 만들고 싶었습니다.  

그렇게 생각난 부분은 충돌체들에게 ID를 부여하고, 충돌 검사시 충돌이 되면 해당 충돌체에게 현재 충돌체와 다른 충돌체 사이의 충돌 정보를 저장하게 하고, 처음 저장된다면 Enter를, 충돌 정보가 있다면 Stay를, 충돌정보가 있는데 이번 프레임에 충돌 검사를 진행하지 않았다면 충돌 검사를 하지 않았으므로 Exit를 호출하도록 구현했습니다.  

그리고 해당 기능을 구현하려고 했던 목적이 Event 처리였기에 함수 포인터를 사용해 콜백 함수를 호출할 수 있도록 구현했습니다.  

    void SetCallbackOnCollisionEnter(std::function<void(OUT Collider*)> const& Event) { m_callbackOnCollisionEnter = Event; };
    void SetCallbackOnCollisionStay(std::function<void(OUT Collider*)> const& Event) { m_callbackOnCollisionStay = Event; };
    void SetCallbackOnCollisionExit(std::function<void(OUT Collider*)> const& Event) { m_callbackOnCollisionExit = Event; };
    ... 
    map<UINT, CollisionCheck> m_collisionMap; // (충돌 정보 저장용)
~~생각보다 잘 동작해 기쁘고 뿌듯했었습니다...~~  

### 5) 애니메이션 프레임 콜백 (Anim Notify)
충돌 관련 기능을 만들고 근접 무기의 공격 판정을 On/Off 하기 위해 충돌체를 켜고 끌 수 있어야 했습니다.  
이때 제가 생각한 방식은 학원 수업 초기에 1개월 정도 Unreal 엔진을 공부하면서 봤던 Anim Notify 였습니다.  

Anim Notify의 경우 애니메이션 프레임 중간에 애니메이션과 관련되어 처리되어야 할 일을 Anim Notify로 만들어서 호출시킬 수 있게끔 구성되어 있던 것 같습니다.  

그런 기능을 사용하면 공격모션 초기에 무기 충돌체를 On 할 수 있고, 공격모션이 끝나면 무기 충돌체를 Off 시킬 수 있을 거라 생각했습니다.  

따라서 매 프레임마다 콜백 함수를 저장할 수 있는 함수 포인터를 넣어두고 필요한 위치에 콜백 함수를 넣을 수 있도록 구현했습니다.  

    struct Frame
    {
    	Texture* texture;
    	float    duration;
    	std::function<void()> callbackEvent;
    };

다행히 원하던 On/Off 기능은 잘 돌아갔습니다.  

다만 이 애니메이션 프레임 콜백은 애니메이션이 해당 프레임에 도달해야 함수를 호출하게 되어있고, 도중에 애니메이션이 끊기거나 다른 애니메이션으로 전환되면 프리징 현상이나 다른 버그들이 발생했습니다.  

이를 어떻게든 해결하고 싶었습니다.  

애니메이션과 다르게 상태만 따로 처리하는 방법도 있었지만, 애니메이션이 변경되었을 때, 부자연스러움이 발생할 것 같아 최대한 애니메이션과 맞추고 싶었습니다.  

그러다보니 콜백 애니메이션이 필요한 애니메이션을 재생중이라면 해당 애니메이션을 멈추면 안된다고 생각했던 것 같습니다.  

현재 구현된 방식은 캐릭터에게 현재 상태를 나타내는 state 변수와 다른 필요한 bool 변수들을 만들어두고 멈추면 안되는 애니메이션이 재생중이라면 애니메이션을 멈추지 않게 구현되어 있습니다.  

그러다보니 플레이 영상에서 공격 모션 중에는 적의 공격에 맞더라도 맞는 모션이 재생되지 않게 되어있습니다.  

이 부분은 좀 더 공부하고 좀 더 좋은 아이디어를 찾아봐야 할 것 같습니다.  

그래도 현재 애니메이션 프레임 콜백을 구현함으로써 애니메이션 동작이 끝날 때 필요한 함수를 호출하거나 시작할 때 필요한 함수를 호출할 수 있게 구현되어 있습니다.  

예를 들면, 문으로 막힌 플랫폼은 애니메이션의 어느 부분에 도달하면 통과될 수 있도록 충돌체를 Off 시킨다거나, 바닥이 꺼지는 플랫폼은 애니메이션의 시작 부분에선 충돌체를 On 시켜주고 중간에 충돌체를 Off 시켜주는 단순 동작들은 잘 동작하고 있습니다.  


### 6) 관찰자 (Observer)
무기 상태 UI나 생명력 같은 UI를 구현할 때 플레이어의 정보가 필요했습니다.  
이 부분은 GameObject를 주입받을지, 필요한 정보만 가지고 있을지 고민했습니다.  

그러다가 관찰자 패턴이라는 것을 읽게 되었습니다. 상태가 변경되면 유튜브 알림처럼 구독한 대상들에게 알림을 보내는 패턴이었습니다.  
이것을 이용해 플레이어에게 상태 변경이 일어나면 구독하고 있는 모든 옵저버에게 구분 가능한 문자열과 자기 자신을 보내고 필요한 요소에서 참조할 수 있도록 만들었습니다.

	template <typename T>
    class Observer
    {
    public:
        virtual void FieldChanged(T& field, const string& fieldName) = 0;
    };

    template <typename T>
    class Observable
    {
    public:
        Observable() = default;
        ~Observable()
        {
            observers.clear();
        }
    
    private:
        std::vector<Observer<T>*> observers = {};
    
    public:
        void Notify(T& field, const string& fieldName)
        {
            for (auto observer : observers)
            {
                observer->FieldChanged(field, fieldName);
            }
        }
        void Subscribe(Observer<T>* observer)
        {
            observers.emplace_back(observer);
        }
        void Unsubscribe(Observer<T>* observer)
        {
            observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        }
    
    };

- UI의 경우 렌더링 마지막에서 DirectX에 넘겨주는 View를 초기화하여 단위행렬을 View로 넘겨주는 방식으로 개발했습니다.

### 7) 그 외...
#### ① 컴포넌트
위의 애니메이션이나 충돌체, 중력은 컴포넌트로 구현했습니다.  
따라서 같은 GameObject를 상속받아도 중력이 필요없는 플랫폼의 경우 중력의 영향을 받지 않고, Player나 Monster의 경우 중력의 영향을 받습니다.    

#### ② Pool
플레이어의 총알과 총알이 터지는 이펙트처럼 자주 생성되고 사라지는 객체의 경우 미리 몇 개 만들어 사용하는 객체 풀 패턴을 사용했습니다.   

#### ③ 상태 (State)
몬스터의 AI의 경우 유한 상태 패턴을 사용하여 구현했습니다.   
이때, 몬스터의 죽음 같은 모든 상태 진행 이전에 검사되어야 하는 상태의 경우 우선 검사할 수 있도록 먼저 검사할 수 있도록 GLOBAL이라는 상태를 두었습니다.   

    template <typename T, typename E>
    class AI
    {
	...
        void Update()
        {
            if (m_globalState != nullptr)
            {
                m_globalState->Update(m_owner);
            }
            
            if (m_currentState != nullptr)
            {
                m_currentState->Update(m_owner);
            }
        }
    ...
	}