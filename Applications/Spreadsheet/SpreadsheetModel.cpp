/*
 * Copyright (c) 2020, the SerenityOS developers.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SpreadsheetModel.h"

namespace Spreadsheet {

SheetModel::~SheetModel()
{
}

GUI::Variant SheetModel::data(const GUI::ModelIndex& index, GUI::ModelRole role) const
{
    if (!index.is_valid())
        return {};

    if (role == GUI::ModelRole::Display) {
        if (index.column() == 0)
            return String::number(index.row());

        const auto* value = m_sheet->at({ m_sheet->column(index.column() - 1), (size_t)index.row() });
        if (!value)
            return String::empty();

        if (value->kind == Spreadsheet::Cell::Formula)
            return value->evaluated_data.is_empty() ? "" : value->evaluated_data.to_string_without_side_effects();

        return value->data;
    }

    if (role == GUI::ModelRole::TextAlignment) {
        if (index.column() == 0)
            return {};

        return {};
    }

    return {};
}

String SheetModel::column_name(int index) const
{
    if (index < 0)
        return {};

    if (index == 0)
        return "";

    return m_sheet->column(index - 1);
}

bool SheetModel::is_editable(const GUI::ModelIndex& index) const
{
    if (!index.is_valid())
        return false;

    if (index.column() == 0)
        return false;

    return true;
}

void SheetModel::set_data(const GUI::ModelIndex& index, const GUI::Variant& value)
{
    if (!index.is_valid())
        return;

    if (index.column() == 0)
        return;

    auto& cell = m_sheet->ensure({ m_sheet->column(index.column() - 1), (size_t)index.row() });
    cell.set_data(value.to_string());
    update();
}

void SheetModel::update()
{
    m_sheet->update();
}

}
