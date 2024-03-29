﻿#pragma once
#include <memory>
#include <Siv3D/Array.hpp>
#include <Siv3D/HashTable.hpp>

namespace ct::Migration
{
    struct IMigration
    {
    public:
        virtual ~IMigration() = default;
        virtual void up() const = 0;
        virtual void down() const = 0;
    };

    template<s3d::int64 Id>
    struct Migration;

    class MigrationHundler
    {
    public:
        static s3d::Array<s3d::int64> Versions();
        static s3d::Array<s3d::int64> UpdateVersions(s3d::int64 currentVersion);

        template<s3d::int64 Id>
        static void Regist(IMigration* migration)
        {
            m_migrations[Id] = migration;
        }

        static IMigration* Get(s3d::int64 version);
    private:
        MigrationHundler() = default;
        MigrationHundler(const MigrationHundler& other) = delete;
        void operator=(const MigrationHundler& other) = delete;
    private:
        inline static s3d::HashTable<s3d::int64, IMigration*> m_migrations;
    };

    template<s3d::int64 Id>
    struct Migration : public IMigration
    {
    public:
        Migration()
        {
            MigrationHundler::Regist<Id>(this);
        }
    };

    namespace detail
    {
        consteval s3d::int64 versionId(const char* path)
        {
            // filename取得
            const char* filename = path;
            while (*path) {
                if (*path++ == '\\') {
                    filename = path;
                }
            }
            // バージョン値取得
            const char* str = filename;
            s3d::int64 value = 0;
            while (char c = *str) {
                if (c <= '9' && c >= '0') {
                    value = (c - '0') + value * 10;
                } else {
                    break;
                }
                ++str;
            }
            return value;
        }
    }
}
#define MIGRATION(name) struct name :  ::ct::Migration::Migration<::ct::Migration::detail::versionId(__FILE__)>
