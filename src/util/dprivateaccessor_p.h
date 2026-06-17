// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <cstddef>
#include <QtCore/qcompilerdetection.h>

// Private member accessor using the explicit template instantiation technique.
//
// C++ Standard [temp.explicit]/12 states:
// "The usual access checking rules do not apply to names used to
// specify explicit instantiation definitions."
//
// This allows passing pointers to private/protected data members and
// member functions as template arguments in explicit instantiations,
// bypassing normal access control — without modifying the class definition
// and without the UB caused by "#define private public".
//
// NOTE: These helper structs must be in the SAME namespace as the Tag structs
// (global namespace, since the macros expand at file scope). If they were in a
// sub-namespace, the friend definition would create a different function than
// the friend declaration in the Tag struct, causing undefined-reference errors.

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wnon-template-friend")

template<typename Tag>
struct DtkDeclarativePrivateAccessor
{
    using MemberPtr = typename Tag::MemberPtr;
    friend constexpr MemberPtr get(Tag) noexcept;
};

template<typename Tag, typename Tag::MemberPtr Ptr>
struct DtkDeclarativePrivateAccessorImpl : DtkDeclarativePrivateAccessor<Tag>
{
    friend constexpr typename Tag::MemberPtr get(Tag) noexcept { return Ptr; }
};

QT_WARNING_POP

#define D_DECLARE_PRIVATE_MEMBER(TagName, ClassName, Member, MemberType) \
    struct TagName { \
        using MemberPtr = MemberType ClassName::*; \
        friend constexpr MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkDeclarativePrivateAccessorImpl<TagName, &ClassName::Member>

#define D_DECLARE_PRIVATE_METHOD(TagName, ClassName, MethodName, RetType, ...) \
    struct TagName { \
        using MemberPtr = RetType (ClassName::*)(__VA_ARGS__); \
        friend constexpr MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkDeclarativePrivateAccessorImpl<TagName, &ClassName::MethodName>

#define D_DECLARE_PRIVATE_CONST_METHOD(TagName, ClassName, MethodName, RetType, ...) \
    struct TagName { \
        using MemberPtr = RetType (ClassName::*)(__VA_ARGS__) const; \
        friend constexpr MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkDeclarativePrivateAccessorImpl<TagName, &ClassName::MethodName>

#define D_DECLARE_PRIVATE_BITFIELD(TagName, ClassName, PrevMember, PrevMemberType, BfStorageType) \
    struct TagName { \
        using MemberPtr = PrevMemberType ClassName::*; \
        friend constexpr MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkDeclarativePrivateAccessorImpl<TagName, &ClassName::PrevMember>; \
    struct TagName##_bf_layout_mirror { PrevMemberType prev; BfStorageType storage; }; \
    struct TagName##_Bits { \
        static constexpr std::size_t kOffset = \
            __builtin_offsetof(TagName##_bf_layout_mirror, storage) - \
            __builtin_offsetof(TagName##_bf_layout_mirror, prev); \
        static BfStorageType *storagePtr(ClassName *obj) noexcept { \
            PrevMemberType &prev_ref = (*obj).*get(TagName{}); \
            return reinterpret_cast<BfStorageType*>( \
                reinterpret_cast<char *>(&prev_ref) + kOffset); \
        } \
        static const BfStorageType *storagePtr(const ClassName *obj) noexcept { \
            return storagePtr(const_cast<ClassName *>(obj)); \
        } \
        static bool getBit(const ClassName *obj, unsigned bit) noexcept { \
            return (*storagePtr(obj) >> bit) & BfStorageType{1}; \
        } \
        static void setBit(ClassName *obj, unsigned bit, bool val) noexcept { \
            BfStorageType *p = storagePtr(obj); \
            if (val) *p |= (BfStorageType{1} << bit); \
            else     *p &= ~(BfStorageType{1} << bit); \
        } \
    }

// Trampoline: ensures get(tag) is called from a context with no class-scope
// get() member that might suppress ADL (C++ [basic.lookup.argdep] para 3).
namespace dtk_private_detail {
    template<typename Tag>
    inline typename Tag::MemberPtr access(Tag t) noexcept { return get(t); }
}

#define D_PRIVATE_MEMBER(obj, tag) ((obj).*dtk_private_detail::access(tag))
#define D_PRIVATE_CALL(obj, tag, ...) ((obj).*dtk_private_detail::access(tag))(__VA_ARGS__)
#define D_PRIVATE_BF_GET(obj, TagName, bit_pos) \
    TagName##_Bits::getBit(&(obj), (bit_pos))
#define D_PRIVATE_BF_SET(obj, TagName, bit_pos, val) \
    TagName##_Bits::setBit(&(obj), (bit_pos), (val))
