#pragma once
#ifndef CATA_SRC_MATTACK_COMMON_H
#define CATA_SRC_MATTACK_COMMON_H

#include <string> // IWYU pragma: keep
#include <memory>
#include <type_traits>

#include "condition.h"
#include "clone_ptr.h"
#include "creature.h"
#include "dialogue.h"
#include "type_id.h"

class JsonObject;
class monster;

using mattack_id = std::string;
using mon_action_attack = bool ( * )( monster * );

class mattack_actor
{
    protected:
        mattack_actor() = default;
    public:
        explicit mattack_actor( const mattack_id &new_id ) : id( new_id ) { }

        mattack_id id;
        bool was_loaded = false;

        int cooldown = 0;
        // Percent chance for the attack to happen if the mob tries it
        int attack_chance = 100;

        // Dialogue conditions of the attack
        std::function<bool( const dialogue & )> condition;
        bool has_condition = false;

        void load( const JsonObject &jo, const std::string &src );

        virtual ~mattack_actor() = default;
        virtual bool call( monster & ) const = 0;
        virtual std::unique_ptr<mattack_actor> clone() const = 0;
        virtual void load_internal( const JsonObject &jo, const std::string &src ) = 0;
};

struct mtype_special_attack {
    protected:
        // TODO: Remove friend
        friend struct mtype;
        cata::clone_ptr<mattack_actor> actor;

    public:
        mtype_special_attack( const mattack_id &id, mon_action_attack f );
        explicit mtype_special_attack( std::unique_ptr<mattack_actor> f ) : actor( std::move( f ) ) { }

        const mattack_actor &operator*() const {
            return *actor;
        }

        const mattack_actor *operator->() const {
            return actor.get();
        }

        const mattack_actor *get() const {
            return actor.get();
        }
};

#endif // CATA_SRC_MATTACK_COMMON_H
