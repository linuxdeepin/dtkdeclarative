/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <dtkcore_global.h>

#include <QtCore/QMetaMethod>

#define DQUICK_NAMESPACE Quick
#define DTK_QUICK_NAMESPACE DTK_NAMESPACE::Quick

#define DQUICK_BEGIN_NAMESPACE namespace DTK_NAMESPACE { namespace DQUICK_NAMESPACE {
#define DQUICK_END_NAMESPACE }}
#define DQUICK_USE_NAMESPACE using namespace DTK_QUICK_NAMESPACE;
