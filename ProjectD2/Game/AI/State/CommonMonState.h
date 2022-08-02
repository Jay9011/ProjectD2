#pragma once
#include "Engine/AI/State/State.h"
class Monster;
namespace CommonMonState
{
	class Idle : public State<Monster>
	{
		void Enter(Monster* _entity) override;
		void Update(Monster* _entity) override;
		void Exit(Monster* _entity) override;

		MON_STATE GetType() const override { return MON_STATE::IDLE; }
	};
	class Patrol : public State<Monster>
	{
		void Enter(Monster* _entity) override;
		void Update(Monster* _entity) override;
		void Exit(Monster* _entity) override;

		MON_STATE GetType() const override { return MON_STATE::PATROL; }
	};
	class Chase : public State<Monster>
	{
		void Enter(Monster* _entity) override;
		void Update(Monster* _entity) override;
		void Exit(Monster* _entity) override;

		MON_STATE GetType() const override { return MON_STATE::CHASE; }
	};
	class Attack : public State<Monster>
	{
		void Enter(Monster* _entity) override;
		void Update(Monster* _entity) override;
		void Exit(Monster* _entity) override;

		MON_STATE GetType() const override { return MON_STATE::ATTACK; }
	};
	class Die : public State<Monster>
	{
		void Enter(Monster* _entity) override;
		void Update(Monster* _entity) override;
		void Exit(Monster* _entity) override;

		MON_STATE GetType() const override { return MON_STATE::DIE; }
	};
	class Global : public State<Monster>
	{
		void Enter(Monster* _entity) override;
		void Update(Monster* _entity) override;
		void Exit(Monster* _entity) override;

		MON_STATE GetType() const override { return MON_STATE::GLOBAL; }
	};
}