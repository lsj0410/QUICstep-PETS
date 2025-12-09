#ifndef SRC_TRACE_PROCESSOR_TABLES_SLICE_TABLES_PY_H_
#define SRC_TRACE_PROCESSOR_TABLES_SLICE_TABLES_PY_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "perfetto/base/logging.h"
#include "perfetto/trace_processor/basic_types.h"
#include "perfetto/trace_processor/ref_counted.h"
#include "src/trace_processor/containers/bit_vector.h"
#include "src/trace_processor/containers/row_map.h"
#include "src/trace_processor/containers/string_pool.h"
#include "src/trace_processor/db/column/arrangement_overlay.h"
#include "src/trace_processor/db/column/data_layer.h"
#include "src/trace_processor/db/column/dense_null_overlay.h"
#include "src/trace_processor/db/column/numeric_storage.h"
#include "src/trace_processor/db/column/id_storage.h"
#include "src/trace_processor/db/column/null_overlay.h"
#include "src/trace_processor/db/column/range_overlay.h"
#include "src/trace_processor/db/column/selector_overlay.h"
#include "src/trace_processor/db/column/set_id_storage.h"
#include "src/trace_processor/db/column/string_storage.h"
#include "src/trace_processor/db/column/types.h"
#include "src/trace_processor/db/column_storage.h"
#include "src/trace_processor/db/column.h"
#include "src/trace_processor/db/table.h"
#include "src/trace_processor/db/typed_column.h"
#include "src/trace_processor/db/typed_column_internal.h"
#include "src/trace_processor/tables/macros_internal.h"

#include "src/trace_processor/tables/track_tables_py.h"

namespace perfetto::trace_processor::tables {

class SliceTable : public macros_internal::MacroTable {
 public:
  static constexpr uint32_t kColumnCount = 16;

  struct Id : public BaseId {
    Id() = default;
    explicit constexpr Id(uint32_t v) : BaseId(v) {}
  };
  static_assert(std::is_trivially_destructible_v<Id>,
                "Inheritance used without trivial destruction");
    
  struct ColumnIndex {
    static constexpr uint32_t id = 0;
    static constexpr uint32_t type = 1;
    static constexpr uint32_t ts = 2;
    static constexpr uint32_t dur = 3;
    static constexpr uint32_t track_id = 4;
    static constexpr uint32_t category = 5;
    static constexpr uint32_t name = 6;
    static constexpr uint32_t depth = 7;
    static constexpr uint32_t stack_id = 8;
    static constexpr uint32_t parent_stack_id = 9;
    static constexpr uint32_t parent_id = 10;
    static constexpr uint32_t arg_set_id = 11;
    static constexpr uint32_t thread_ts = 12;
    static constexpr uint32_t thread_dur = 13;
    static constexpr uint32_t thread_instruction_count = 14;
    static constexpr uint32_t thread_instruction_delta = 15;
  };
  struct ColumnType {
    using id = IdColumn<SliceTable::Id>;
    using type = TypedColumn<StringPool::Id>;
    using ts = TypedColumn<int64_t>;
    using dur = TypedColumn<int64_t>;
    using track_id = TypedColumn<TrackTable::Id>;
    using category = TypedColumn<std::optional<StringPool::Id>>;
    using name = TypedColumn<std::optional<StringPool::Id>>;
    using depth = TypedColumn<uint32_t>;
    using stack_id = TypedColumn<int64_t>;
    using parent_stack_id = TypedColumn<int64_t>;
    using parent_id = TypedColumn<std::optional<SliceTable::Id>>;
    using arg_set_id = TypedColumn<uint32_t>;
    using thread_ts = TypedColumn<std::optional<int64_t>>;
    using thread_dur = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_count = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_delta = TypedColumn<std::optional<int64_t>>;
  };
  struct Row : public macros_internal::RootParentTable::Row {
    Row(int64_t in_ts = {},
        int64_t in_dur = {},
        TrackTable::Id in_track_id = {},
        std::optional<StringPool::Id> in_category = {},
        std::optional<StringPool::Id> in_name = {},
        uint32_t in_depth = {},
        int64_t in_stack_id = {},
        int64_t in_parent_stack_id = {},
        std::optional<SliceTable::Id> in_parent_id = {},
        uint32_t in_arg_set_id = {},
        std::optional<int64_t> in_thread_ts = {},
        std::optional<int64_t> in_thread_dur = {},
        std::optional<int64_t> in_thread_instruction_count = {},
        std::optional<int64_t> in_thread_instruction_delta = {},
        std::nullptr_t = nullptr)
        : macros_internal::RootParentTable::Row(),
          ts(in_ts),
          dur(in_dur),
          track_id(in_track_id),
          category(in_category),
          name(in_name),
          depth(in_depth),
          stack_id(in_stack_id),
          parent_stack_id(in_parent_stack_id),
          parent_id(in_parent_id),
          arg_set_id(in_arg_set_id),
          thread_ts(in_thread_ts),
          thread_dur(in_thread_dur),
          thread_instruction_count(in_thread_instruction_count),
          thread_instruction_delta(in_thread_instruction_delta) {
      type_ = "internal_slice";
    }
    int64_t ts;
    int64_t dur;
    TrackTable::Id track_id;
    std::optional<StringPool::Id> category;
    std::optional<StringPool::Id> name;
    uint32_t depth;
    int64_t stack_id;
    int64_t parent_stack_id;
    std::optional<SliceTable::Id> parent_id;
    uint32_t arg_set_id;
    std::optional<int64_t> thread_ts;
    std::optional<int64_t> thread_dur;
    std::optional<int64_t> thread_instruction_count;
    std::optional<int64_t> thread_instruction_delta;

    bool operator==(const SliceTable::Row& other) const {
      return type() == other.type() && ColumnType::ts::Equals(ts, other.ts) &&
       ColumnType::dur::Equals(dur, other.dur) &&
       ColumnType::track_id::Equals(track_id, other.track_id) &&
       ColumnType::category::Equals(category, other.category) &&
       ColumnType::name::Equals(name, other.name) &&
       ColumnType::depth::Equals(depth, other.depth) &&
       ColumnType::stack_id::Equals(stack_id, other.stack_id) &&
       ColumnType::parent_stack_id::Equals(parent_stack_id, other.parent_stack_id) &&
       ColumnType::parent_id::Equals(parent_id, other.parent_id) &&
       ColumnType::arg_set_id::Equals(arg_set_id, other.arg_set_id) &&
       ColumnType::thread_ts::Equals(thread_ts, other.thread_ts) &&
       ColumnType::thread_dur::Equals(thread_dur, other.thread_dur) &&
       ColumnType::thread_instruction_count::Equals(thread_instruction_count, other.thread_instruction_count) &&
       ColumnType::thread_instruction_delta::Equals(thread_instruction_delta, other.thread_instruction_delta);
    }
  };
  struct ColumnFlag {
    static constexpr uint32_t ts = static_cast<uint32_t>(ColumnLegacy::Flag::kSorted) | ColumnType::ts::default_flags();
    static constexpr uint32_t dur = ColumnType::dur::default_flags();
    static constexpr uint32_t track_id = ColumnType::track_id::default_flags();
    static constexpr uint32_t category = ColumnType::category::default_flags();
    static constexpr uint32_t name = ColumnType::name::default_flags();
    static constexpr uint32_t depth = ColumnType::depth::default_flags();
    static constexpr uint32_t stack_id = ColumnType::stack_id::default_flags();
    static constexpr uint32_t parent_stack_id = ColumnType::parent_stack_id::default_flags();
    static constexpr uint32_t parent_id = ColumnType::parent_id::default_flags();
    static constexpr uint32_t arg_set_id = ColumnType::arg_set_id::default_flags();
    static constexpr uint32_t thread_ts = ColumnType::thread_ts::default_flags();
    static constexpr uint32_t thread_dur = ColumnType::thread_dur::default_flags();
    static constexpr uint32_t thread_instruction_count = ColumnType::thread_instruction_count::default_flags();
    static constexpr uint32_t thread_instruction_delta = ColumnType::thread_instruction_delta::default_flags();
  };

  class RowNumber;
  class ConstRowReference;
  class RowReference;

  class RowNumber : public macros_internal::AbstractRowNumber<
      SliceTable, ConstRowReference, RowReference> {
   public:
    explicit RowNumber(uint32_t row_number)
        : AbstractRowNumber(row_number) {}
  };
  static_assert(std::is_trivially_destructible_v<RowNumber>,
                "Inheritance used without trivial destruction");

  class ConstRowReference : public macros_internal::AbstractConstRowReference<
    SliceTable, RowNumber> {
   public:
    ConstRowReference(const SliceTable* table, uint32_t row_number)
        : AbstractConstRowReference(table, row_number) {}

    ColumnType::id::type id() const {
      return table_->id()[row_number_];
    }
    ColumnType::type::type type() const {
      return table_->type()[row_number_];
    }
    ColumnType::ts::type ts() const {
      return table_->ts()[row_number_];
    }
    ColumnType::dur::type dur() const {
      return table_->dur()[row_number_];
    }
    ColumnType::track_id::type track_id() const {
      return table_->track_id()[row_number_];
    }
    ColumnType::category::type category() const {
      return table_->category()[row_number_];
    }
    ColumnType::name::type name() const {
      return table_->name()[row_number_];
    }
    ColumnType::depth::type depth() const {
      return table_->depth()[row_number_];
    }
    ColumnType::stack_id::type stack_id() const {
      return table_->stack_id()[row_number_];
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      return table_->parent_stack_id()[row_number_];
    }
    ColumnType::parent_id::type parent_id() const {
      return table_->parent_id()[row_number_];
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      return table_->arg_set_id()[row_number_];
    }
    ColumnType::thread_ts::type thread_ts() const {
      return table_->thread_ts()[row_number_];
    }
    ColumnType::thread_dur::type thread_dur() const {
      return table_->thread_dur()[row_number_];
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      return table_->thread_instruction_count()[row_number_];
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      return table_->thread_instruction_delta()[row_number_];
    }
  };
  static_assert(std::is_trivially_destructible_v<ConstRowReference>,
                "Inheritance used without trivial destruction");
  class RowReference : public ConstRowReference {
   public:
    RowReference(const SliceTable* table, uint32_t row_number)
        : ConstRowReference(table, row_number) {}

    void set_ts(
        ColumnType::ts::non_optional_type v) {
      return mutable_table()->mutable_ts()->Set(row_number_, v);
    }
    void set_dur(
        ColumnType::dur::non_optional_type v) {
      return mutable_table()->mutable_dur()->Set(row_number_, v);
    }
    void set_track_id(
        ColumnType::track_id::non_optional_type v) {
      return mutable_table()->mutable_track_id()->Set(row_number_, v);
    }
    void set_category(
        ColumnType::category::non_optional_type v) {
      return mutable_table()->mutable_category()->Set(row_number_, v);
    }
    void set_name(
        ColumnType::name::non_optional_type v) {
      return mutable_table()->mutable_name()->Set(row_number_, v);
    }
    void set_depth(
        ColumnType::depth::non_optional_type v) {
      return mutable_table()->mutable_depth()->Set(row_number_, v);
    }
    void set_stack_id(
        ColumnType::stack_id::non_optional_type v) {
      return mutable_table()->mutable_stack_id()->Set(row_number_, v);
    }
    void set_parent_stack_id(
        ColumnType::parent_stack_id::non_optional_type v) {
      return mutable_table()->mutable_parent_stack_id()->Set(row_number_, v);
    }
    void set_parent_id(
        ColumnType::parent_id::non_optional_type v) {
      return mutable_table()->mutable_parent_id()->Set(row_number_, v);
    }
    void set_arg_set_id(
        ColumnType::arg_set_id::non_optional_type v) {
      return mutable_table()->mutable_arg_set_id()->Set(row_number_, v);
    }
    void set_thread_ts(
        ColumnType::thread_ts::non_optional_type v) {
      return mutable_table()->mutable_thread_ts()->Set(row_number_, v);
    }
    void set_thread_dur(
        ColumnType::thread_dur::non_optional_type v) {
      return mutable_table()->mutable_thread_dur()->Set(row_number_, v);
    }
    void set_thread_instruction_count(
        ColumnType::thread_instruction_count::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_count()->Set(row_number_, v);
    }
    void set_thread_instruction_delta(
        ColumnType::thread_instruction_delta::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_delta()->Set(row_number_, v);
    }

   private:
    SliceTable* mutable_table() const {
      return const_cast<SliceTable*>(table_);
    }
  };
  static_assert(std::is_trivially_destructible_v<RowReference>,
                "Inheritance used without trivial destruction");

  class ConstIterator;
  class ConstIterator : public macros_internal::AbstractConstIterator<
    ConstIterator, SliceTable, RowNumber, ConstRowReference> {
   public:
    ColumnType::id::type id() const {
      const auto& col = table_->id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::type::type type() const {
      const auto& col = table_->type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::ts::type ts() const {
      const auto& col = table_->ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::dur::type dur() const {
      const auto& col = table_->dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::track_id::type track_id() const {
      const auto& col = table_->track_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::category::type category() const {
      const auto& col = table_->category();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::name::type name() const {
      const auto& col = table_->name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::depth::type depth() const {
      const auto& col = table_->depth();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::stack_id::type stack_id() const {
      const auto& col = table_->stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      const auto& col = table_->parent_stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_id::type parent_id() const {
      const auto& col = table_->parent_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      const auto& col = table_->arg_set_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_ts::type thread_ts() const {
      const auto& col = table_->thread_ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_dur::type thread_dur() const {
      const auto& col = table_->thread_dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      const auto& col = table_->thread_instruction_count();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      const auto& col = table_->thread_instruction_delta();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }

   protected:
    explicit ConstIterator(const SliceTable* table,
                           Table::Iterator iterator)
        : AbstractConstIterator(table, std::move(iterator)) {}

    uint32_t CurrentRowNumber() const {
      return iterator_.StorageIndexForLastOverlay();
    }

   private:
    friend class SliceTable;
    friend class macros_internal::AbstractConstIterator<
      ConstIterator, SliceTable, RowNumber, ConstRowReference>;
  };
  class Iterator : public ConstIterator {
    public:
     void set_ts(ColumnType::ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_dur(ColumnType::dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_track_id(ColumnType::track_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_track_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_category(ColumnType::category::non_optional_type v) {
        auto* col = mutable_table_->mutable_category();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_name(ColumnType::name::non_optional_type v) {
        auto* col = mutable_table_->mutable_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_depth(ColumnType::depth::non_optional_type v) {
        auto* col = mutable_table_->mutable_depth();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_stack_id(ColumnType::stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_stack_id(ColumnType::parent_stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_id(ColumnType::parent_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_arg_set_id(ColumnType::arg_set_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_arg_set_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_ts(ColumnType::thread_ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_dur(ColumnType::thread_dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_count(ColumnType::thread_instruction_count::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_count();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_delta(ColumnType::thread_instruction_delta::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_delta();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }

     RowReference row_reference() const {
       return RowReference(mutable_table_, CurrentRowNumber());
     }

    private:
     friend class SliceTable;

     explicit Iterator(SliceTable* table, Table::Iterator iterator)
        : ConstIterator(table, std::move(iterator)),
          mutable_table_(table) {}

     SliceTable* mutable_table_ = nullptr;
  };

  struct IdAndRow {
    Id id;
    uint32_t row;
    RowReference row_reference;
    RowNumber row_number;
  };

  static std::vector<ColumnLegacy> GetColumns(
      SliceTable* self,
      const macros_internal::MacroTable* parent) {
    std::vector<ColumnLegacy> columns =
        CopyColumnsFromParentOrAddRootColumns(self, parent);
    uint32_t olay_idx = OverlayCount(parent);
    AddColumnToVector(columns, "ts", &self->ts_, ColumnFlag::ts,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "dur", &self->dur_, ColumnFlag::dur,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "track_id", &self->track_id_, ColumnFlag::track_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "category", &self->category_, ColumnFlag::category,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "name", &self->name_, ColumnFlag::name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "depth", &self->depth_, ColumnFlag::depth,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "stack_id", &self->stack_id_, ColumnFlag::stack_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "parent_stack_id", &self->parent_stack_id_, ColumnFlag::parent_stack_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "parent_id", &self->parent_id_, ColumnFlag::parent_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "arg_set_id", &self->arg_set_id_, ColumnFlag::arg_set_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "thread_ts", &self->thread_ts_, ColumnFlag::thread_ts,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "thread_dur", &self->thread_dur_, ColumnFlag::thread_dur,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "thread_instruction_count", &self->thread_instruction_count_, ColumnFlag::thread_instruction_count,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "thread_instruction_delta", &self->thread_instruction_delta_, ColumnFlag::thread_instruction_delta,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    return columns;
  }

  PERFETTO_NO_INLINE explicit SliceTable(StringPool* pool)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, nullptr),
          nullptr),
        ts_(ColumnStorage<ColumnType::ts::stored_type>::Create<false>()),
        dur_(ColumnStorage<ColumnType::dur::stored_type>::Create<false>()),
        track_id_(ColumnStorage<ColumnType::track_id::stored_type>::Create<false>()),
        category_(ColumnStorage<ColumnType::category::stored_type>::Create<false>()),
        name_(ColumnStorage<ColumnType::name::stored_type>::Create<false>()),
        depth_(ColumnStorage<ColumnType::depth::stored_type>::Create<false>()),
        stack_id_(ColumnStorage<ColumnType::stack_id::stored_type>::Create<false>()),
        parent_stack_id_(ColumnStorage<ColumnType::parent_stack_id::stored_type>::Create<false>()),
        parent_id_(ColumnStorage<ColumnType::parent_id::stored_type>::Create<false>()),
        arg_set_id_(ColumnStorage<ColumnType::arg_set_id::stored_type>::Create<false>()),
        thread_ts_(ColumnStorage<ColumnType::thread_ts::stored_type>::Create<false>()),
        thread_dur_(ColumnStorage<ColumnType::thread_dur::stored_type>::Create<false>()),
        thread_instruction_count_(ColumnStorage<ColumnType::thread_instruction_count::stored_type>::Create<false>()),
        thread_instruction_delta_(ColumnStorage<ColumnType::thread_instruction_delta::stored_type>::Create<false>())
,
        id_storage_layer_(new column::IdStorage()),
        type_storage_layer_(
          new column::StringStorage(string_pool(), &type_.vector())),
        ts_storage_layer_(
        new column::NumericStorage<ColumnType::ts::non_optional_stored_type>(
          &ts_.vector(),
          ColumnTypeHelper<ColumnType::ts::stored_type>::ToColumnType(),
          true)),
        dur_storage_layer_(
        new column::NumericStorage<ColumnType::dur::non_optional_stored_type>(
          &dur_.vector(),
          ColumnTypeHelper<ColumnType::dur::stored_type>::ToColumnType(),
          false)),
        track_id_storage_layer_(
        new column::NumericStorage<ColumnType::track_id::non_optional_stored_type>(
          &track_id_.vector(),
          ColumnTypeHelper<ColumnType::track_id::stored_type>::ToColumnType(),
          false)),
        category_storage_layer_(
          new column::StringStorage(string_pool(), &category_.vector())),
        name_storage_layer_(
          new column::StringStorage(string_pool(), &name_.vector())),
        depth_storage_layer_(
        new column::NumericStorage<ColumnType::depth::non_optional_stored_type>(
          &depth_.vector(),
          ColumnTypeHelper<ColumnType::depth::stored_type>::ToColumnType(),
          false)),
        stack_id_storage_layer_(
        new column::NumericStorage<ColumnType::stack_id::non_optional_stored_type>(
          &stack_id_.vector(),
          ColumnTypeHelper<ColumnType::stack_id::stored_type>::ToColumnType(),
          false)),
        parent_stack_id_storage_layer_(
        new column::NumericStorage<ColumnType::parent_stack_id::non_optional_stored_type>(
          &parent_stack_id_.vector(),
          ColumnTypeHelper<ColumnType::parent_stack_id::stored_type>::ToColumnType(),
          false)),
        parent_id_storage_layer_(
          new column::NumericStorage<ColumnType::parent_id::non_optional_stored_type>(
            &parent_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::parent_id::stored_type>::ToColumnType(),
            false)),
        arg_set_id_storage_layer_(
        new column::NumericStorage<ColumnType::arg_set_id::non_optional_stored_type>(
          &arg_set_id_.vector(),
          ColumnTypeHelper<ColumnType::arg_set_id::stored_type>::ToColumnType(),
          false)),
        thread_ts_storage_layer_(
          new column::NumericStorage<ColumnType::thread_ts::non_optional_stored_type>(
            &thread_ts_.non_null_vector(),
            ColumnTypeHelper<ColumnType::thread_ts::stored_type>::ToColumnType(),
            false)),
        thread_dur_storage_layer_(
          new column::NumericStorage<ColumnType::thread_dur::non_optional_stored_type>(
            &thread_dur_.non_null_vector(),
            ColumnTypeHelper<ColumnType::thread_dur::stored_type>::ToColumnType(),
            false)),
        thread_instruction_count_storage_layer_(
          new column::NumericStorage<ColumnType::thread_instruction_count::non_optional_stored_type>(
            &thread_instruction_count_.non_null_vector(),
            ColumnTypeHelper<ColumnType::thread_instruction_count::stored_type>::ToColumnType(),
            false)),
        thread_instruction_delta_storage_layer_(
          new column::NumericStorage<ColumnType::thread_instruction_delta::non_optional_stored_type>(
            &thread_instruction_delta_.non_null_vector(),
            ColumnTypeHelper<ColumnType::thread_instruction_delta::stored_type>::ToColumnType(),
            false))
,
        parent_id_null_layer_(new column::NullOverlay(parent_id_.bv())),
        thread_ts_null_layer_(new column::NullOverlay(thread_ts_.bv())),
        thread_dur_null_layer_(new column::NullOverlay(thread_dur_.bv())),
        thread_instruction_count_null_layer_(new column::NullOverlay(thread_instruction_count_.bv())),
        thread_instruction_delta_null_layer_(new column::NullOverlay(thread_instruction_delta_.bv())) {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::ts::stored_type>(
          ColumnFlag::ts),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::dur::stored_type>(
          ColumnFlag::dur),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::track_id::stored_type>(
          ColumnFlag::track_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::category::stored_type>(
          ColumnFlag::category),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::name::stored_type>(
          ColumnFlag::name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::depth::stored_type>(
          ColumnFlag::depth),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::stack_id::stored_type>(
          ColumnFlag::stack_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::parent_stack_id::stored_type>(
          ColumnFlag::parent_stack_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::parent_id::stored_type>(
          ColumnFlag::parent_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::arg_set_id::stored_type>(
          ColumnFlag::arg_set_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::thread_ts::stored_type>(
          ColumnFlag::thread_ts),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::thread_dur::stored_type>(
          ColumnFlag::thread_dur),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::thread_instruction_count::stored_type>(
          ColumnFlag::thread_instruction_count),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::thread_instruction_delta::stored_type>(
          ColumnFlag::thread_instruction_delta),
        "Column type and flag combination is not valid");
    OnConstructionCompletedRegularConstructor(
      {id_storage_layer_,type_storage_layer_,ts_storage_layer_,dur_storage_layer_,track_id_storage_layer_,category_storage_layer_,name_storage_layer_,depth_storage_layer_,stack_id_storage_layer_,parent_stack_id_storage_layer_,parent_id_storage_layer_,arg_set_id_storage_layer_,thread_ts_storage_layer_,thread_dur_storage_layer_,thread_instruction_count_storage_layer_,thread_instruction_delta_storage_layer_},
      {{},{},{},{},{},{},{},{},{},{},parent_id_null_layer_,{},thread_ts_null_layer_,thread_dur_null_layer_,thread_instruction_count_null_layer_,thread_instruction_delta_null_layer_});
  }
  ~SliceTable() override;

  static const char* Name() { return "internal_slice"; }

  static Table::Schema ComputeStaticSchema() {
    Table::Schema schema;
    schema.columns.emplace_back(Table::Schema::Column{
        "id", SqlValue::Type::kLong, true, true, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "type", SqlValue::Type::kString, false, false, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "ts", ColumnType::ts::SqlValueType(), false,
        true,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "dur", ColumnType::dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "track_id", ColumnType::track_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "category", ColumnType::category::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "name", ColumnType::name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "depth", ColumnType::depth::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "stack_id", ColumnType::stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_stack_id", ColumnType::parent_stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_id", ColumnType::parent_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "arg_set_id", ColumnType::arg_set_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_ts", ColumnType::thread_ts::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_dur", ColumnType::thread_dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_count", ColumnType::thread_instruction_count::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_delta", ColumnType::thread_instruction_delta::SqlValueType(), false,
        false,
        false,
        false});
    return schema;
  }

  ConstIterator IterateRows() const {
    return ConstIterator(this, Table::IterateRows());
  }

  Iterator IterateRows() { return Iterator(this, Table::IterateRows()); }

  ConstIterator FilterToIterator(
      const std::vector<Constraint>& cs) const {
    return ConstIterator(
      this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  Iterator FilterToIterator(
      const std::vector<Constraint>& cs) {
    return Iterator(this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  void ShrinkToFit() {
    type_.ShrinkToFit();
    ts_.ShrinkToFit();
    dur_.ShrinkToFit();
    track_id_.ShrinkToFit();
    category_.ShrinkToFit();
    name_.ShrinkToFit();
    depth_.ShrinkToFit();
    stack_id_.ShrinkToFit();
    parent_stack_id_.ShrinkToFit();
    parent_id_.ShrinkToFit();
    arg_set_id_.ShrinkToFit();
    thread_ts_.ShrinkToFit();
    thread_dur_.ShrinkToFit();
    thread_instruction_count_.ShrinkToFit();
    thread_instruction_delta_.ShrinkToFit();
  }

  std::optional<ConstRowReference> FindById(Id find_id) const {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(ConstRowReference(this, *row))
               : std::nullopt;
  }

  std::optional<RowReference> FindById(Id find_id) {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(RowReference(this, *row)) : std::nullopt;
  }

  IdAndRow Insert(const Row& row) {
    uint32_t row_number = row_count();
    Id id = Id{row_number};
    type_.Append(string_pool()->InternString(row.type()));
    mutable_ts()->Append(row.ts);
    mutable_dur()->Append(row.dur);
    mutable_track_id()->Append(row.track_id);
    mutable_category()->Append(row.category);
    mutable_name()->Append(row.name);
    mutable_depth()->Append(row.depth);
    mutable_stack_id()->Append(row.stack_id);
    mutable_parent_stack_id()->Append(row.parent_stack_id);
    mutable_parent_id()->Append(row.parent_id);
    mutable_arg_set_id()->Append(row.arg_set_id);
    mutable_thread_ts()->Append(row.thread_ts);
    mutable_thread_dur()->Append(row.thread_dur);
    mutable_thread_instruction_count()->Append(row.thread_instruction_count);
    mutable_thread_instruction_delta()->Append(row.thread_instruction_delta);
    UpdateSelfOverlayAfterInsert();
    return IdAndRow{id, row_number, RowReference(this, row_number),
                     RowNumber(row_number)};
  }

  

  const IdColumn<SliceTable::Id>& id() const {
    return static_cast<const ColumnType::id&>(columns()[ColumnIndex::id]);
  }
  const TypedColumn<StringPool::Id>& type() const {
    return static_cast<const ColumnType::type&>(columns()[ColumnIndex::type]);
  }
  const TypedColumn<int64_t>& ts() const {
    return static_cast<const ColumnType::ts&>(columns()[ColumnIndex::ts]);
  }
  const TypedColumn<int64_t>& dur() const {
    return static_cast<const ColumnType::dur&>(columns()[ColumnIndex::dur]);
  }
  const TypedColumn<TrackTable::Id>& track_id() const {
    return static_cast<const ColumnType::track_id&>(columns()[ColumnIndex::track_id]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& category() const {
    return static_cast<const ColumnType::category&>(columns()[ColumnIndex::category]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& name() const {
    return static_cast<const ColumnType::name&>(columns()[ColumnIndex::name]);
  }
  const TypedColumn<uint32_t>& depth() const {
    return static_cast<const ColumnType::depth&>(columns()[ColumnIndex::depth]);
  }
  const TypedColumn<int64_t>& stack_id() const {
    return static_cast<const ColumnType::stack_id&>(columns()[ColumnIndex::stack_id]);
  }
  const TypedColumn<int64_t>& parent_stack_id() const {
    return static_cast<const ColumnType::parent_stack_id&>(columns()[ColumnIndex::parent_stack_id]);
  }
  const TypedColumn<std::optional<SliceTable::Id>>& parent_id() const {
    return static_cast<const ColumnType::parent_id&>(columns()[ColumnIndex::parent_id]);
  }
  const TypedColumn<uint32_t>& arg_set_id() const {
    return static_cast<const ColumnType::arg_set_id&>(columns()[ColumnIndex::arg_set_id]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_ts() const {
    return static_cast<const ColumnType::thread_ts&>(columns()[ColumnIndex::thread_ts]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_dur() const {
    return static_cast<const ColumnType::thread_dur&>(columns()[ColumnIndex::thread_dur]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_count() const {
    return static_cast<const ColumnType::thread_instruction_count&>(columns()[ColumnIndex::thread_instruction_count]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_delta() const {
    return static_cast<const ColumnType::thread_instruction_delta&>(columns()[ColumnIndex::thread_instruction_delta]);
  }

  TypedColumn<int64_t>* mutable_ts() {
    return static_cast<ColumnType::ts*>(
        GetColumn(ColumnIndex::ts));
  }
  TypedColumn<int64_t>* mutable_dur() {
    return static_cast<ColumnType::dur*>(
        GetColumn(ColumnIndex::dur));
  }
  TypedColumn<TrackTable::Id>* mutable_track_id() {
    return static_cast<ColumnType::track_id*>(
        GetColumn(ColumnIndex::track_id));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_category() {
    return static_cast<ColumnType::category*>(
        GetColumn(ColumnIndex::category));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_name() {
    return static_cast<ColumnType::name*>(
        GetColumn(ColumnIndex::name));
  }
  TypedColumn<uint32_t>* mutable_depth() {
    return static_cast<ColumnType::depth*>(
        GetColumn(ColumnIndex::depth));
  }
  TypedColumn<int64_t>* mutable_stack_id() {
    return static_cast<ColumnType::stack_id*>(
        GetColumn(ColumnIndex::stack_id));
  }
  TypedColumn<int64_t>* mutable_parent_stack_id() {
    return static_cast<ColumnType::parent_stack_id*>(
        GetColumn(ColumnIndex::parent_stack_id));
  }
  TypedColumn<std::optional<SliceTable::Id>>* mutable_parent_id() {
    return static_cast<ColumnType::parent_id*>(
        GetColumn(ColumnIndex::parent_id));
  }
  TypedColumn<uint32_t>* mutable_arg_set_id() {
    return static_cast<ColumnType::arg_set_id*>(
        GetColumn(ColumnIndex::arg_set_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_ts() {
    return static_cast<ColumnType::thread_ts*>(
        GetColumn(ColumnIndex::thread_ts));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_dur() {
    return static_cast<ColumnType::thread_dur*>(
        GetColumn(ColumnIndex::thread_dur));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_count() {
    return static_cast<ColumnType::thread_instruction_count*>(
        GetColumn(ColumnIndex::thread_instruction_count));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_delta() {
    return static_cast<ColumnType::thread_instruction_delta*>(
        GetColumn(ColumnIndex::thread_instruction_delta));
  }

 private:
  
  
  ColumnStorage<ColumnType::ts::stored_type> ts_;
  ColumnStorage<ColumnType::dur::stored_type> dur_;
  ColumnStorage<ColumnType::track_id::stored_type> track_id_;
  ColumnStorage<ColumnType::category::stored_type> category_;
  ColumnStorage<ColumnType::name::stored_type> name_;
  ColumnStorage<ColumnType::depth::stored_type> depth_;
  ColumnStorage<ColumnType::stack_id::stored_type> stack_id_;
  ColumnStorage<ColumnType::parent_stack_id::stored_type> parent_stack_id_;
  ColumnStorage<ColumnType::parent_id::stored_type> parent_id_;
  ColumnStorage<ColumnType::arg_set_id::stored_type> arg_set_id_;
  ColumnStorage<ColumnType::thread_ts::stored_type> thread_ts_;
  ColumnStorage<ColumnType::thread_dur::stored_type> thread_dur_;
  ColumnStorage<ColumnType::thread_instruction_count::stored_type> thread_instruction_count_;
  ColumnStorage<ColumnType::thread_instruction_delta::stored_type> thread_instruction_delta_;

  RefPtr<column::DataLayer> id_storage_layer_;
  RefPtr<column::DataLayer> type_storage_layer_;
  RefPtr<column::DataLayer> ts_storage_layer_;
  RefPtr<column::DataLayer> dur_storage_layer_;
  RefPtr<column::DataLayer> track_id_storage_layer_;
  RefPtr<column::DataLayer> category_storage_layer_;
  RefPtr<column::DataLayer> name_storage_layer_;
  RefPtr<column::DataLayer> depth_storage_layer_;
  RefPtr<column::DataLayer> stack_id_storage_layer_;
  RefPtr<column::DataLayer> parent_stack_id_storage_layer_;
  RefPtr<column::DataLayer> parent_id_storage_layer_;
  RefPtr<column::DataLayer> arg_set_id_storage_layer_;
  RefPtr<column::DataLayer> thread_ts_storage_layer_;
  RefPtr<column::DataLayer> thread_dur_storage_layer_;
  RefPtr<column::DataLayer> thread_instruction_count_storage_layer_;
  RefPtr<column::DataLayer> thread_instruction_delta_storage_layer_;

  RefPtr<column::DataLayer> parent_id_null_layer_;
  RefPtr<column::DataLayer> thread_ts_null_layer_;
  RefPtr<column::DataLayer> thread_dur_null_layer_;
  RefPtr<column::DataLayer> thread_instruction_count_null_layer_;
  RefPtr<column::DataLayer> thread_instruction_delta_null_layer_;
};
  

class ActualFrameTimelineSliceTable : public macros_internal::MacroTable {
 public:
  static constexpr uint32_t kColumnCount = 27;

  using Id = SliceTable::Id;
    
  struct ColumnIndex {
    static constexpr uint32_t id = 0;
    static constexpr uint32_t type = 1;
    static constexpr uint32_t ts = 2;
    static constexpr uint32_t dur = 3;
    static constexpr uint32_t track_id = 4;
    static constexpr uint32_t category = 5;
    static constexpr uint32_t name = 6;
    static constexpr uint32_t depth = 7;
    static constexpr uint32_t stack_id = 8;
    static constexpr uint32_t parent_stack_id = 9;
    static constexpr uint32_t parent_id = 10;
    static constexpr uint32_t arg_set_id = 11;
    static constexpr uint32_t thread_ts = 12;
    static constexpr uint32_t thread_dur = 13;
    static constexpr uint32_t thread_instruction_count = 14;
    static constexpr uint32_t thread_instruction_delta = 15;
    static constexpr uint32_t display_frame_token = 16;
    static constexpr uint32_t surface_frame_token = 17;
    static constexpr uint32_t upid = 18;
    static constexpr uint32_t layer_name = 19;
    static constexpr uint32_t present_type = 20;
    static constexpr uint32_t on_time_finish = 21;
    static constexpr uint32_t gpu_composition = 22;
    static constexpr uint32_t jank_type = 23;
    static constexpr uint32_t jank_severity_type = 24;
    static constexpr uint32_t prediction_type = 25;
    static constexpr uint32_t jank_tag = 26;
  };
  struct ColumnType {
    using id = IdColumn<ActualFrameTimelineSliceTable::Id>;
    using type = TypedColumn<StringPool::Id>;
    using ts = TypedColumn<int64_t>;
    using dur = TypedColumn<int64_t>;
    using track_id = TypedColumn<TrackTable::Id>;
    using category = TypedColumn<std::optional<StringPool::Id>>;
    using name = TypedColumn<std::optional<StringPool::Id>>;
    using depth = TypedColumn<uint32_t>;
    using stack_id = TypedColumn<int64_t>;
    using parent_stack_id = TypedColumn<int64_t>;
    using parent_id = TypedColumn<std::optional<ActualFrameTimelineSliceTable::Id>>;
    using arg_set_id = TypedColumn<uint32_t>;
    using thread_ts = TypedColumn<std::optional<int64_t>>;
    using thread_dur = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_count = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_delta = TypedColumn<std::optional<int64_t>>;
    using display_frame_token = TypedColumn<int64_t>;
    using surface_frame_token = TypedColumn<int64_t>;
    using upid = TypedColumn<uint32_t>;
    using layer_name = TypedColumn<StringPool::Id>;
    using present_type = TypedColumn<StringPool::Id>;
    using on_time_finish = TypedColumn<int32_t>;
    using gpu_composition = TypedColumn<int32_t>;
    using jank_type = TypedColumn<StringPool::Id>;
    using jank_severity_type = TypedColumn<StringPool::Id>;
    using prediction_type = TypedColumn<StringPool::Id>;
    using jank_tag = TypedColumn<StringPool::Id>;
  };
  struct Row : public SliceTable::Row {
    Row(int64_t in_ts = {},
        int64_t in_dur = {},
        TrackTable::Id in_track_id = {},
        std::optional<StringPool::Id> in_category = {},
        std::optional<StringPool::Id> in_name = {},
        uint32_t in_depth = {},
        int64_t in_stack_id = {},
        int64_t in_parent_stack_id = {},
        std::optional<ActualFrameTimelineSliceTable::Id> in_parent_id = {},
        uint32_t in_arg_set_id = {},
        std::optional<int64_t> in_thread_ts = {},
        std::optional<int64_t> in_thread_dur = {},
        std::optional<int64_t> in_thread_instruction_count = {},
        std::optional<int64_t> in_thread_instruction_delta = {},
        int64_t in_display_frame_token = {},
        int64_t in_surface_frame_token = {},
        uint32_t in_upid = {},
        StringPool::Id in_layer_name = {},
        StringPool::Id in_present_type = {},
        int32_t in_on_time_finish = {},
        int32_t in_gpu_composition = {},
        StringPool::Id in_jank_type = {},
        StringPool::Id in_jank_severity_type = {},
        StringPool::Id in_prediction_type = {},
        StringPool::Id in_jank_tag = {},
        std::nullptr_t = nullptr)
        : SliceTable::Row(in_ts, in_dur, in_track_id, in_category, in_name, in_depth, in_stack_id, in_parent_stack_id, in_parent_id, in_arg_set_id, in_thread_ts, in_thread_dur, in_thread_instruction_count, in_thread_instruction_delta),
          display_frame_token(in_display_frame_token),
          surface_frame_token(in_surface_frame_token),
          upid(in_upid),
          layer_name(in_layer_name),
          present_type(in_present_type),
          on_time_finish(in_on_time_finish),
          gpu_composition(in_gpu_composition),
          jank_type(in_jank_type),
          jank_severity_type(in_jank_severity_type),
          prediction_type(in_prediction_type),
          jank_tag(in_jank_tag) {
      type_ = "actual_frame_timeline_slice";
    }
    int64_t display_frame_token;
    int64_t surface_frame_token;
    uint32_t upid;
    StringPool::Id layer_name;
    StringPool::Id present_type;
    int32_t on_time_finish;
    int32_t gpu_composition;
    StringPool::Id jank_type;
    StringPool::Id jank_severity_type;
    StringPool::Id prediction_type;
    StringPool::Id jank_tag;

    bool operator==(const ActualFrameTimelineSliceTable::Row& other) const {
      return type() == other.type() && ColumnType::ts::Equals(ts, other.ts) &&
       ColumnType::dur::Equals(dur, other.dur) &&
       ColumnType::track_id::Equals(track_id, other.track_id) &&
       ColumnType::category::Equals(category, other.category) &&
       ColumnType::name::Equals(name, other.name) &&
       ColumnType::depth::Equals(depth, other.depth) &&
       ColumnType::stack_id::Equals(stack_id, other.stack_id) &&
       ColumnType::parent_stack_id::Equals(parent_stack_id, other.parent_stack_id) &&
       ColumnType::parent_id::Equals(parent_id, other.parent_id) &&
       ColumnType::arg_set_id::Equals(arg_set_id, other.arg_set_id) &&
       ColumnType::thread_ts::Equals(thread_ts, other.thread_ts) &&
       ColumnType::thread_dur::Equals(thread_dur, other.thread_dur) &&
       ColumnType::thread_instruction_count::Equals(thread_instruction_count, other.thread_instruction_count) &&
       ColumnType::thread_instruction_delta::Equals(thread_instruction_delta, other.thread_instruction_delta) &&
       ColumnType::display_frame_token::Equals(display_frame_token, other.display_frame_token) &&
       ColumnType::surface_frame_token::Equals(surface_frame_token, other.surface_frame_token) &&
       ColumnType::upid::Equals(upid, other.upid) &&
       ColumnType::layer_name::Equals(layer_name, other.layer_name) &&
       ColumnType::present_type::Equals(present_type, other.present_type) &&
       ColumnType::on_time_finish::Equals(on_time_finish, other.on_time_finish) &&
       ColumnType::gpu_composition::Equals(gpu_composition, other.gpu_composition) &&
       ColumnType::jank_type::Equals(jank_type, other.jank_type) &&
       ColumnType::jank_severity_type::Equals(jank_severity_type, other.jank_severity_type) &&
       ColumnType::prediction_type::Equals(prediction_type, other.prediction_type) &&
       ColumnType::jank_tag::Equals(jank_tag, other.jank_tag);
    }
  };
  struct ColumnFlag {
    static constexpr uint32_t display_frame_token = ColumnType::display_frame_token::default_flags();
    static constexpr uint32_t surface_frame_token = ColumnType::surface_frame_token::default_flags();
    static constexpr uint32_t upid = ColumnType::upid::default_flags();
    static constexpr uint32_t layer_name = ColumnType::layer_name::default_flags();
    static constexpr uint32_t present_type = ColumnType::present_type::default_flags();
    static constexpr uint32_t on_time_finish = ColumnType::on_time_finish::default_flags();
    static constexpr uint32_t gpu_composition = ColumnType::gpu_composition::default_flags();
    static constexpr uint32_t jank_type = ColumnType::jank_type::default_flags();
    static constexpr uint32_t jank_severity_type = ColumnType::jank_severity_type::default_flags();
    static constexpr uint32_t prediction_type = ColumnType::prediction_type::default_flags();
    static constexpr uint32_t jank_tag = ColumnType::jank_tag::default_flags();
  };

  class RowNumber;
  class ConstRowReference;
  class RowReference;

  class RowNumber : public macros_internal::AbstractRowNumber<
      ActualFrameTimelineSliceTable, ConstRowReference, RowReference> {
   public:
    explicit RowNumber(uint32_t row_number)
        : AbstractRowNumber(row_number) {}
  };
  static_assert(std::is_trivially_destructible_v<RowNumber>,
                "Inheritance used without trivial destruction");

  class ConstRowReference : public macros_internal::AbstractConstRowReference<
    ActualFrameTimelineSliceTable, RowNumber> {
   public:
    ConstRowReference(const ActualFrameTimelineSliceTable* table, uint32_t row_number)
        : AbstractConstRowReference(table, row_number) {}

    ColumnType::id::type id() const {
      return table_->id()[row_number_];
    }
    ColumnType::type::type type() const {
      return table_->type()[row_number_];
    }
    ColumnType::ts::type ts() const {
      return table_->ts()[row_number_];
    }
    ColumnType::dur::type dur() const {
      return table_->dur()[row_number_];
    }
    ColumnType::track_id::type track_id() const {
      return table_->track_id()[row_number_];
    }
    ColumnType::category::type category() const {
      return table_->category()[row_number_];
    }
    ColumnType::name::type name() const {
      return table_->name()[row_number_];
    }
    ColumnType::depth::type depth() const {
      return table_->depth()[row_number_];
    }
    ColumnType::stack_id::type stack_id() const {
      return table_->stack_id()[row_number_];
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      return table_->parent_stack_id()[row_number_];
    }
    ColumnType::parent_id::type parent_id() const {
      return table_->parent_id()[row_number_];
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      return table_->arg_set_id()[row_number_];
    }
    ColumnType::thread_ts::type thread_ts() const {
      return table_->thread_ts()[row_number_];
    }
    ColumnType::thread_dur::type thread_dur() const {
      return table_->thread_dur()[row_number_];
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      return table_->thread_instruction_count()[row_number_];
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      return table_->thread_instruction_delta()[row_number_];
    }
    ColumnType::display_frame_token::type display_frame_token() const {
      return table_->display_frame_token()[row_number_];
    }
    ColumnType::surface_frame_token::type surface_frame_token() const {
      return table_->surface_frame_token()[row_number_];
    }
    ColumnType::upid::type upid() const {
      return table_->upid()[row_number_];
    }
    ColumnType::layer_name::type layer_name() const {
      return table_->layer_name()[row_number_];
    }
    ColumnType::present_type::type present_type() const {
      return table_->present_type()[row_number_];
    }
    ColumnType::on_time_finish::type on_time_finish() const {
      return table_->on_time_finish()[row_number_];
    }
    ColumnType::gpu_composition::type gpu_composition() const {
      return table_->gpu_composition()[row_number_];
    }
    ColumnType::jank_type::type jank_type() const {
      return table_->jank_type()[row_number_];
    }
    ColumnType::jank_severity_type::type jank_severity_type() const {
      return table_->jank_severity_type()[row_number_];
    }
    ColumnType::prediction_type::type prediction_type() const {
      return table_->prediction_type()[row_number_];
    }
    ColumnType::jank_tag::type jank_tag() const {
      return table_->jank_tag()[row_number_];
    }
  };
  static_assert(std::is_trivially_destructible_v<ConstRowReference>,
                "Inheritance used without trivial destruction");
  class RowReference : public ConstRowReference {
   public:
    RowReference(const ActualFrameTimelineSliceTable* table, uint32_t row_number)
        : ConstRowReference(table, row_number) {}

    void set_ts(
        ColumnType::ts::non_optional_type v) {
      return mutable_table()->mutable_ts()->Set(row_number_, v);
    }
    void set_dur(
        ColumnType::dur::non_optional_type v) {
      return mutable_table()->mutable_dur()->Set(row_number_, v);
    }
    void set_track_id(
        ColumnType::track_id::non_optional_type v) {
      return mutable_table()->mutable_track_id()->Set(row_number_, v);
    }
    void set_category(
        ColumnType::category::non_optional_type v) {
      return mutable_table()->mutable_category()->Set(row_number_, v);
    }
    void set_name(
        ColumnType::name::non_optional_type v) {
      return mutable_table()->mutable_name()->Set(row_number_, v);
    }
    void set_depth(
        ColumnType::depth::non_optional_type v) {
      return mutable_table()->mutable_depth()->Set(row_number_, v);
    }
    void set_stack_id(
        ColumnType::stack_id::non_optional_type v) {
      return mutable_table()->mutable_stack_id()->Set(row_number_, v);
    }
    void set_parent_stack_id(
        ColumnType::parent_stack_id::non_optional_type v) {
      return mutable_table()->mutable_parent_stack_id()->Set(row_number_, v);
    }
    void set_parent_id(
        ColumnType::parent_id::non_optional_type v) {
      return mutable_table()->mutable_parent_id()->Set(row_number_, v);
    }
    void set_arg_set_id(
        ColumnType::arg_set_id::non_optional_type v) {
      return mutable_table()->mutable_arg_set_id()->Set(row_number_, v);
    }
    void set_thread_ts(
        ColumnType::thread_ts::non_optional_type v) {
      return mutable_table()->mutable_thread_ts()->Set(row_number_, v);
    }
    void set_thread_dur(
        ColumnType::thread_dur::non_optional_type v) {
      return mutable_table()->mutable_thread_dur()->Set(row_number_, v);
    }
    void set_thread_instruction_count(
        ColumnType::thread_instruction_count::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_count()->Set(row_number_, v);
    }
    void set_thread_instruction_delta(
        ColumnType::thread_instruction_delta::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_delta()->Set(row_number_, v);
    }
    void set_display_frame_token(
        ColumnType::display_frame_token::non_optional_type v) {
      return mutable_table()->mutable_display_frame_token()->Set(row_number_, v);
    }
    void set_surface_frame_token(
        ColumnType::surface_frame_token::non_optional_type v) {
      return mutable_table()->mutable_surface_frame_token()->Set(row_number_, v);
    }
    void set_upid(
        ColumnType::upid::non_optional_type v) {
      return mutable_table()->mutable_upid()->Set(row_number_, v);
    }
    void set_layer_name(
        ColumnType::layer_name::non_optional_type v) {
      return mutable_table()->mutable_layer_name()->Set(row_number_, v);
    }
    void set_present_type(
        ColumnType::present_type::non_optional_type v) {
      return mutable_table()->mutable_present_type()->Set(row_number_, v);
    }
    void set_on_time_finish(
        ColumnType::on_time_finish::non_optional_type v) {
      return mutable_table()->mutable_on_time_finish()->Set(row_number_, v);
    }
    void set_gpu_composition(
        ColumnType::gpu_composition::non_optional_type v) {
      return mutable_table()->mutable_gpu_composition()->Set(row_number_, v);
    }
    void set_jank_type(
        ColumnType::jank_type::non_optional_type v) {
      return mutable_table()->mutable_jank_type()->Set(row_number_, v);
    }
    void set_jank_severity_type(
        ColumnType::jank_severity_type::non_optional_type v) {
      return mutable_table()->mutable_jank_severity_type()->Set(row_number_, v);
    }
    void set_prediction_type(
        ColumnType::prediction_type::non_optional_type v) {
      return mutable_table()->mutable_prediction_type()->Set(row_number_, v);
    }
    void set_jank_tag(
        ColumnType::jank_tag::non_optional_type v) {
      return mutable_table()->mutable_jank_tag()->Set(row_number_, v);
    }

   private:
    ActualFrameTimelineSliceTable* mutable_table() const {
      return const_cast<ActualFrameTimelineSliceTable*>(table_);
    }
  };
  static_assert(std::is_trivially_destructible_v<RowReference>,
                "Inheritance used without trivial destruction");

  class ConstIterator;
  class ConstIterator : public macros_internal::AbstractConstIterator<
    ConstIterator, ActualFrameTimelineSliceTable, RowNumber, ConstRowReference> {
   public:
    ColumnType::id::type id() const {
      const auto& col = table_->id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::type::type type() const {
      const auto& col = table_->type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::ts::type ts() const {
      const auto& col = table_->ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::dur::type dur() const {
      const auto& col = table_->dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::track_id::type track_id() const {
      const auto& col = table_->track_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::category::type category() const {
      const auto& col = table_->category();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::name::type name() const {
      const auto& col = table_->name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::depth::type depth() const {
      const auto& col = table_->depth();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::stack_id::type stack_id() const {
      const auto& col = table_->stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      const auto& col = table_->parent_stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_id::type parent_id() const {
      const auto& col = table_->parent_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      const auto& col = table_->arg_set_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_ts::type thread_ts() const {
      const auto& col = table_->thread_ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_dur::type thread_dur() const {
      const auto& col = table_->thread_dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      const auto& col = table_->thread_instruction_count();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      const auto& col = table_->thread_instruction_delta();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::display_frame_token::type display_frame_token() const {
      const auto& col = table_->display_frame_token();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::surface_frame_token::type surface_frame_token() const {
      const auto& col = table_->surface_frame_token();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::upid::type upid() const {
      const auto& col = table_->upid();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::layer_name::type layer_name() const {
      const auto& col = table_->layer_name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::present_type::type present_type() const {
      const auto& col = table_->present_type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::on_time_finish::type on_time_finish() const {
      const auto& col = table_->on_time_finish();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::gpu_composition::type gpu_composition() const {
      const auto& col = table_->gpu_composition();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::jank_type::type jank_type() const {
      const auto& col = table_->jank_type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::jank_severity_type::type jank_severity_type() const {
      const auto& col = table_->jank_severity_type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::prediction_type::type prediction_type() const {
      const auto& col = table_->prediction_type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::jank_tag::type jank_tag() const {
      const auto& col = table_->jank_tag();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }

   protected:
    explicit ConstIterator(const ActualFrameTimelineSliceTable* table,
                           Table::Iterator iterator)
        : AbstractConstIterator(table, std::move(iterator)) {}

    uint32_t CurrentRowNumber() const {
      return iterator_.StorageIndexForLastOverlay();
    }

   private:
    friend class ActualFrameTimelineSliceTable;
    friend class macros_internal::AbstractConstIterator<
      ConstIterator, ActualFrameTimelineSliceTable, RowNumber, ConstRowReference>;
  };
  class Iterator : public ConstIterator {
    public:
     void set_ts(ColumnType::ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_dur(ColumnType::dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_track_id(ColumnType::track_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_track_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_category(ColumnType::category::non_optional_type v) {
        auto* col = mutable_table_->mutable_category();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_name(ColumnType::name::non_optional_type v) {
        auto* col = mutable_table_->mutable_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_depth(ColumnType::depth::non_optional_type v) {
        auto* col = mutable_table_->mutable_depth();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_stack_id(ColumnType::stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_stack_id(ColumnType::parent_stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_id(ColumnType::parent_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_arg_set_id(ColumnType::arg_set_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_arg_set_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_ts(ColumnType::thread_ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_dur(ColumnType::thread_dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_count(ColumnType::thread_instruction_count::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_count();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_delta(ColumnType::thread_instruction_delta::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_delta();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_display_frame_token(ColumnType::display_frame_token::non_optional_type v) {
        auto* col = mutable_table_->mutable_display_frame_token();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_surface_frame_token(ColumnType::surface_frame_token::non_optional_type v) {
        auto* col = mutable_table_->mutable_surface_frame_token();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_upid(ColumnType::upid::non_optional_type v) {
        auto* col = mutable_table_->mutable_upid();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_layer_name(ColumnType::layer_name::non_optional_type v) {
        auto* col = mutable_table_->mutable_layer_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_present_type(ColumnType::present_type::non_optional_type v) {
        auto* col = mutable_table_->mutable_present_type();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_on_time_finish(ColumnType::on_time_finish::non_optional_type v) {
        auto* col = mutable_table_->mutable_on_time_finish();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_gpu_composition(ColumnType::gpu_composition::non_optional_type v) {
        auto* col = mutable_table_->mutable_gpu_composition();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_jank_type(ColumnType::jank_type::non_optional_type v) {
        auto* col = mutable_table_->mutable_jank_type();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_jank_severity_type(ColumnType::jank_severity_type::non_optional_type v) {
        auto* col = mutable_table_->mutable_jank_severity_type();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_prediction_type(ColumnType::prediction_type::non_optional_type v) {
        auto* col = mutable_table_->mutable_prediction_type();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_jank_tag(ColumnType::jank_tag::non_optional_type v) {
        auto* col = mutable_table_->mutable_jank_tag();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }

     RowReference row_reference() const {
       return RowReference(mutable_table_, CurrentRowNumber());
     }

    private:
     friend class ActualFrameTimelineSliceTable;

     explicit Iterator(ActualFrameTimelineSliceTable* table, Table::Iterator iterator)
        : ConstIterator(table, std::move(iterator)),
          mutable_table_(table) {}

     ActualFrameTimelineSliceTable* mutable_table_ = nullptr;
  };

  struct IdAndRow {
    Id id;
    uint32_t row;
    RowReference row_reference;
    RowNumber row_number;
  };

  static std::vector<ColumnLegacy> GetColumns(
      ActualFrameTimelineSliceTable* self,
      const macros_internal::MacroTable* parent) {
    std::vector<ColumnLegacy> columns =
        CopyColumnsFromParentOrAddRootColumns(self, parent);
    uint32_t olay_idx = OverlayCount(parent);
    AddColumnToVector(columns, "display_frame_token", &self->display_frame_token_, ColumnFlag::display_frame_token,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "surface_frame_token", &self->surface_frame_token_, ColumnFlag::surface_frame_token,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "upid", &self->upid_, ColumnFlag::upid,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "layer_name", &self->layer_name_, ColumnFlag::layer_name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "present_type", &self->present_type_, ColumnFlag::present_type,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "on_time_finish", &self->on_time_finish_, ColumnFlag::on_time_finish,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "gpu_composition", &self->gpu_composition_, ColumnFlag::gpu_composition,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "jank_type", &self->jank_type_, ColumnFlag::jank_type,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "jank_severity_type", &self->jank_severity_type_, ColumnFlag::jank_severity_type,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "prediction_type", &self->prediction_type_, ColumnFlag::prediction_type,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "jank_tag", &self->jank_tag_, ColumnFlag::jank_tag,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    return columns;
  }

  PERFETTO_NO_INLINE explicit ActualFrameTimelineSliceTable(StringPool* pool, SliceTable* parent)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, parent),
          parent),
        parent_(parent), const_parent_(parent), display_frame_token_(ColumnStorage<ColumnType::display_frame_token::stored_type>::Create<false>()),
        surface_frame_token_(ColumnStorage<ColumnType::surface_frame_token::stored_type>::Create<false>()),
        upid_(ColumnStorage<ColumnType::upid::stored_type>::Create<false>()),
        layer_name_(ColumnStorage<ColumnType::layer_name::stored_type>::Create<false>()),
        present_type_(ColumnStorage<ColumnType::present_type::stored_type>::Create<false>()),
        on_time_finish_(ColumnStorage<ColumnType::on_time_finish::stored_type>::Create<false>()),
        gpu_composition_(ColumnStorage<ColumnType::gpu_composition::stored_type>::Create<false>()),
        jank_type_(ColumnStorage<ColumnType::jank_type::stored_type>::Create<false>()),
        jank_severity_type_(ColumnStorage<ColumnType::jank_severity_type::stored_type>::Create<false>()),
        prediction_type_(ColumnStorage<ColumnType::prediction_type::stored_type>::Create<false>()),
        jank_tag_(ColumnStorage<ColumnType::jank_tag::stored_type>::Create<false>())
,
        display_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::display_frame_token::non_optional_stored_type>(
          &display_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::display_frame_token::stored_type>::ToColumnType(),
          false)),
        surface_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::surface_frame_token::non_optional_stored_type>(
          &surface_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::surface_frame_token::stored_type>::ToColumnType(),
          false)),
        upid_storage_layer_(
        new column::NumericStorage<ColumnType::upid::non_optional_stored_type>(
          &upid_.vector(),
          ColumnTypeHelper<ColumnType::upid::stored_type>::ToColumnType(),
          false)),
        layer_name_storage_layer_(
          new column::StringStorage(string_pool(), &layer_name_.vector())),
        present_type_storage_layer_(
          new column::StringStorage(string_pool(), &present_type_.vector())),
        on_time_finish_storage_layer_(
        new column::NumericStorage<ColumnType::on_time_finish::non_optional_stored_type>(
          &on_time_finish_.vector(),
          ColumnTypeHelper<ColumnType::on_time_finish::stored_type>::ToColumnType(),
          false)),
        gpu_composition_storage_layer_(
        new column::NumericStorage<ColumnType::gpu_composition::non_optional_stored_type>(
          &gpu_composition_.vector(),
          ColumnTypeHelper<ColumnType::gpu_composition::stored_type>::ToColumnType(),
          false)),
        jank_type_storage_layer_(
          new column::StringStorage(string_pool(), &jank_type_.vector())),
        jank_severity_type_storage_layer_(
          new column::StringStorage(string_pool(), &jank_severity_type_.vector())),
        prediction_type_storage_layer_(
          new column::StringStorage(string_pool(), &prediction_type_.vector())),
        jank_tag_storage_layer_(
          new column::StringStorage(string_pool(), &jank_tag_.vector()))
         {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::display_frame_token::stored_type>(
          ColumnFlag::display_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::surface_frame_token::stored_type>(
          ColumnFlag::surface_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::upid::stored_type>(
          ColumnFlag::upid),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::layer_name::stored_type>(
          ColumnFlag::layer_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::present_type::stored_type>(
          ColumnFlag::present_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::on_time_finish::stored_type>(
          ColumnFlag::on_time_finish),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::gpu_composition::stored_type>(
          ColumnFlag::gpu_composition),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::jank_type::stored_type>(
          ColumnFlag::jank_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::jank_severity_type::stored_type>(
          ColumnFlag::jank_severity_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::prediction_type::stored_type>(
          ColumnFlag::prediction_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::jank_tag::stored_type>(
          ColumnFlag::jank_tag),
        "Column type and flag combination is not valid");
    OnConstructionCompletedRegularConstructor(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],display_frame_token_storage_layer_,surface_frame_token_storage_layer_,upid_storage_layer_,layer_name_storage_layer_,present_type_storage_layer_,on_time_finish_storage_layer_,gpu_composition_storage_layer_,jank_type_storage_layer_,jank_severity_type_storage_layer_,prediction_type_storage_layer_,jank_tag_storage_layer_},
      {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],{},{},{},{},{},{},{},{},{},{},{}});
  }
  ~ActualFrameTimelineSliceTable() override;

  static const char* Name() { return "actual_frame_timeline_slice"; }

  static Table::Schema ComputeStaticSchema() {
    Table::Schema schema;
    schema.columns.emplace_back(Table::Schema::Column{
        "id", SqlValue::Type::kLong, true, true, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "type", SqlValue::Type::kString, false, false, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "ts", ColumnType::ts::SqlValueType(), false,
        true,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "dur", ColumnType::dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "track_id", ColumnType::track_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "category", ColumnType::category::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "name", ColumnType::name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "depth", ColumnType::depth::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "stack_id", ColumnType::stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_stack_id", ColumnType::parent_stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_id", ColumnType::parent_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "arg_set_id", ColumnType::arg_set_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_ts", ColumnType::thread_ts::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_dur", ColumnType::thread_dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_count", ColumnType::thread_instruction_count::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_delta", ColumnType::thread_instruction_delta::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "display_frame_token", ColumnType::display_frame_token::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "surface_frame_token", ColumnType::surface_frame_token::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "upid", ColumnType::upid::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "layer_name", ColumnType::layer_name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "present_type", ColumnType::present_type::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "on_time_finish", ColumnType::on_time_finish::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "gpu_composition", ColumnType::gpu_composition::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "jank_type", ColumnType::jank_type::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "jank_severity_type", ColumnType::jank_severity_type::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "prediction_type", ColumnType::prediction_type::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "jank_tag", ColumnType::jank_tag::SqlValueType(), false,
        false,
        false,
        false});
    return schema;
  }

  ConstIterator IterateRows() const {
    return ConstIterator(this, Table::IterateRows());
  }

  Iterator IterateRows() { return Iterator(this, Table::IterateRows()); }

  ConstIterator FilterToIterator(
      const std::vector<Constraint>& cs) const {
    return ConstIterator(
      this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  Iterator FilterToIterator(
      const std::vector<Constraint>& cs) {
    return Iterator(this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  void ShrinkToFit() {
    display_frame_token_.ShrinkToFit();
    surface_frame_token_.ShrinkToFit();
    upid_.ShrinkToFit();
    layer_name_.ShrinkToFit();
    present_type_.ShrinkToFit();
    on_time_finish_.ShrinkToFit();
    gpu_composition_.ShrinkToFit();
    jank_type_.ShrinkToFit();
    jank_severity_type_.ShrinkToFit();
    prediction_type_.ShrinkToFit();
    jank_tag_.ShrinkToFit();
  }

  std::optional<ConstRowReference> FindById(Id find_id) const {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(ConstRowReference(this, *row))
               : std::nullopt;
  }

  std::optional<RowReference> FindById(Id find_id) {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(RowReference(this, *row)) : std::nullopt;
  }

  IdAndRow Insert(const Row& row) {
    uint32_t row_number = row_count();
    Id id = Id{parent_->Insert(row).id};
    UpdateOverlaysAfterParentInsert();
    mutable_display_frame_token()->Append(row.display_frame_token);
    mutable_surface_frame_token()->Append(row.surface_frame_token);
    mutable_upid()->Append(row.upid);
    mutable_layer_name()->Append(row.layer_name);
    mutable_present_type()->Append(row.present_type);
    mutable_on_time_finish()->Append(row.on_time_finish);
    mutable_gpu_composition()->Append(row.gpu_composition);
    mutable_jank_type()->Append(row.jank_type);
    mutable_jank_severity_type()->Append(row.jank_severity_type);
    mutable_prediction_type()->Append(row.prediction_type);
    mutable_jank_tag()->Append(row.jank_tag);
    UpdateSelfOverlayAfterInsert();
    return IdAndRow{id, row_number, RowReference(this, row_number),
                     RowNumber(row_number)};
  }

  static std::unique_ptr<Table> ExtendParent(
      const SliceTable& parent,
      ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token
, ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token
, ColumnStorage<ColumnType::upid::stored_type> upid
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name
, ColumnStorage<ColumnType::present_type::stored_type> present_type
, ColumnStorage<ColumnType::on_time_finish::stored_type> on_time_finish
, ColumnStorage<ColumnType::gpu_composition::stored_type> gpu_composition
, ColumnStorage<ColumnType::jank_type::stored_type> jank_type
, ColumnStorage<ColumnType::jank_severity_type::stored_type> jank_severity_type
, ColumnStorage<ColumnType::prediction_type::stored_type> prediction_type
, ColumnStorage<ColumnType::jank_tag::stored_type> jank_tag) {
    return std::unique_ptr<Table>(new ActualFrameTimelineSliceTable(
        parent.string_pool(), parent, RowMap(0, parent.row_count()),
        std::move(display_frame_token), std::move(surface_frame_token), std::move(upid), std::move(layer_name), std::move(present_type), std::move(on_time_finish), std::move(gpu_composition), std::move(jank_type), std::move(jank_severity_type), std::move(prediction_type), std::move(jank_tag)));
  }

  static std::unique_ptr<Table> SelectAndExtendParent(
      const SliceTable& parent,
      std::vector<SliceTable::RowNumber> parent_overlay,
      ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token
, ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token
, ColumnStorage<ColumnType::upid::stored_type> upid
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name
, ColumnStorage<ColumnType::present_type::stored_type> present_type
, ColumnStorage<ColumnType::on_time_finish::stored_type> on_time_finish
, ColumnStorage<ColumnType::gpu_composition::stored_type> gpu_composition
, ColumnStorage<ColumnType::jank_type::stored_type> jank_type
, ColumnStorage<ColumnType::jank_severity_type::stored_type> jank_severity_type
, ColumnStorage<ColumnType::prediction_type::stored_type> prediction_type
, ColumnStorage<ColumnType::jank_tag::stored_type> jank_tag) {
    std::vector<uint32_t> prs_untyped(parent_overlay.size());
    for (uint32_t i = 0; i < parent_overlay.size(); ++i) {
      prs_untyped[i] = parent_overlay[i].row_number();
    }
    return std::unique_ptr<Table>(new ActualFrameTimelineSliceTable(
        parent.string_pool(), parent, RowMap(std::move(prs_untyped)),
        std::move(display_frame_token), std::move(surface_frame_token), std::move(upid), std::move(layer_name), std::move(present_type), std::move(on_time_finish), std::move(gpu_composition), std::move(jank_type), std::move(jank_severity_type), std::move(prediction_type), std::move(jank_tag)));
  }

  const IdColumn<ActualFrameTimelineSliceTable::Id>& id() const {
    return static_cast<const ColumnType::id&>(columns()[ColumnIndex::id]);
  }
  const TypedColumn<StringPool::Id>& type() const {
    return static_cast<const ColumnType::type&>(columns()[ColumnIndex::type]);
  }
  const TypedColumn<int64_t>& ts() const {
    return static_cast<const ColumnType::ts&>(columns()[ColumnIndex::ts]);
  }
  const TypedColumn<int64_t>& dur() const {
    return static_cast<const ColumnType::dur&>(columns()[ColumnIndex::dur]);
  }
  const TypedColumn<TrackTable::Id>& track_id() const {
    return static_cast<const ColumnType::track_id&>(columns()[ColumnIndex::track_id]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& category() const {
    return static_cast<const ColumnType::category&>(columns()[ColumnIndex::category]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& name() const {
    return static_cast<const ColumnType::name&>(columns()[ColumnIndex::name]);
  }
  const TypedColumn<uint32_t>& depth() const {
    return static_cast<const ColumnType::depth&>(columns()[ColumnIndex::depth]);
  }
  const TypedColumn<int64_t>& stack_id() const {
    return static_cast<const ColumnType::stack_id&>(columns()[ColumnIndex::stack_id]);
  }
  const TypedColumn<int64_t>& parent_stack_id() const {
    return static_cast<const ColumnType::parent_stack_id&>(columns()[ColumnIndex::parent_stack_id]);
  }
  const TypedColumn<std::optional<ActualFrameTimelineSliceTable::Id>>& parent_id() const {
    return static_cast<const ColumnType::parent_id&>(columns()[ColumnIndex::parent_id]);
  }
  const TypedColumn<uint32_t>& arg_set_id() const {
    return static_cast<const ColumnType::arg_set_id&>(columns()[ColumnIndex::arg_set_id]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_ts() const {
    return static_cast<const ColumnType::thread_ts&>(columns()[ColumnIndex::thread_ts]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_dur() const {
    return static_cast<const ColumnType::thread_dur&>(columns()[ColumnIndex::thread_dur]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_count() const {
    return static_cast<const ColumnType::thread_instruction_count&>(columns()[ColumnIndex::thread_instruction_count]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_delta() const {
    return static_cast<const ColumnType::thread_instruction_delta&>(columns()[ColumnIndex::thread_instruction_delta]);
  }
  const TypedColumn<int64_t>& display_frame_token() const {
    return static_cast<const ColumnType::display_frame_token&>(columns()[ColumnIndex::display_frame_token]);
  }
  const TypedColumn<int64_t>& surface_frame_token() const {
    return static_cast<const ColumnType::surface_frame_token&>(columns()[ColumnIndex::surface_frame_token]);
  }
  const TypedColumn<uint32_t>& upid() const {
    return static_cast<const ColumnType::upid&>(columns()[ColumnIndex::upid]);
  }
  const TypedColumn<StringPool::Id>& layer_name() const {
    return static_cast<const ColumnType::layer_name&>(columns()[ColumnIndex::layer_name]);
  }
  const TypedColumn<StringPool::Id>& present_type() const {
    return static_cast<const ColumnType::present_type&>(columns()[ColumnIndex::present_type]);
  }
  const TypedColumn<int32_t>& on_time_finish() const {
    return static_cast<const ColumnType::on_time_finish&>(columns()[ColumnIndex::on_time_finish]);
  }
  const TypedColumn<int32_t>& gpu_composition() const {
    return static_cast<const ColumnType::gpu_composition&>(columns()[ColumnIndex::gpu_composition]);
  }
  const TypedColumn<StringPool::Id>& jank_type() const {
    return static_cast<const ColumnType::jank_type&>(columns()[ColumnIndex::jank_type]);
  }
  const TypedColumn<StringPool::Id>& jank_severity_type() const {
    return static_cast<const ColumnType::jank_severity_type&>(columns()[ColumnIndex::jank_severity_type]);
  }
  const TypedColumn<StringPool::Id>& prediction_type() const {
    return static_cast<const ColumnType::prediction_type&>(columns()[ColumnIndex::prediction_type]);
  }
  const TypedColumn<StringPool::Id>& jank_tag() const {
    return static_cast<const ColumnType::jank_tag&>(columns()[ColumnIndex::jank_tag]);
  }

  TypedColumn<int64_t>* mutable_ts() {
    return static_cast<ColumnType::ts*>(
        GetColumn(ColumnIndex::ts));
  }
  TypedColumn<int64_t>* mutable_dur() {
    return static_cast<ColumnType::dur*>(
        GetColumn(ColumnIndex::dur));
  }
  TypedColumn<TrackTable::Id>* mutable_track_id() {
    return static_cast<ColumnType::track_id*>(
        GetColumn(ColumnIndex::track_id));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_category() {
    return static_cast<ColumnType::category*>(
        GetColumn(ColumnIndex::category));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_name() {
    return static_cast<ColumnType::name*>(
        GetColumn(ColumnIndex::name));
  }
  TypedColumn<uint32_t>* mutable_depth() {
    return static_cast<ColumnType::depth*>(
        GetColumn(ColumnIndex::depth));
  }
  TypedColumn<int64_t>* mutable_stack_id() {
    return static_cast<ColumnType::stack_id*>(
        GetColumn(ColumnIndex::stack_id));
  }
  TypedColumn<int64_t>* mutable_parent_stack_id() {
    return static_cast<ColumnType::parent_stack_id*>(
        GetColumn(ColumnIndex::parent_stack_id));
  }
  TypedColumn<std::optional<ActualFrameTimelineSliceTable::Id>>* mutable_parent_id() {
    return static_cast<ColumnType::parent_id*>(
        GetColumn(ColumnIndex::parent_id));
  }
  TypedColumn<uint32_t>* mutable_arg_set_id() {
    return static_cast<ColumnType::arg_set_id*>(
        GetColumn(ColumnIndex::arg_set_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_ts() {
    return static_cast<ColumnType::thread_ts*>(
        GetColumn(ColumnIndex::thread_ts));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_dur() {
    return static_cast<ColumnType::thread_dur*>(
        GetColumn(ColumnIndex::thread_dur));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_count() {
    return static_cast<ColumnType::thread_instruction_count*>(
        GetColumn(ColumnIndex::thread_instruction_count));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_delta() {
    return static_cast<ColumnType::thread_instruction_delta*>(
        GetColumn(ColumnIndex::thread_instruction_delta));
  }
  TypedColumn<int64_t>* mutable_display_frame_token() {
    return static_cast<ColumnType::display_frame_token*>(
        GetColumn(ColumnIndex::display_frame_token));
  }
  TypedColumn<int64_t>* mutable_surface_frame_token() {
    return static_cast<ColumnType::surface_frame_token*>(
        GetColumn(ColumnIndex::surface_frame_token));
  }
  TypedColumn<uint32_t>* mutable_upid() {
    return static_cast<ColumnType::upid*>(
        GetColumn(ColumnIndex::upid));
  }
  TypedColumn<StringPool::Id>* mutable_layer_name() {
    return static_cast<ColumnType::layer_name*>(
        GetColumn(ColumnIndex::layer_name));
  }
  TypedColumn<StringPool::Id>* mutable_present_type() {
    return static_cast<ColumnType::present_type*>(
        GetColumn(ColumnIndex::present_type));
  }
  TypedColumn<int32_t>* mutable_on_time_finish() {
    return static_cast<ColumnType::on_time_finish*>(
        GetColumn(ColumnIndex::on_time_finish));
  }
  TypedColumn<int32_t>* mutable_gpu_composition() {
    return static_cast<ColumnType::gpu_composition*>(
        GetColumn(ColumnIndex::gpu_composition));
  }
  TypedColumn<StringPool::Id>* mutable_jank_type() {
    return static_cast<ColumnType::jank_type*>(
        GetColumn(ColumnIndex::jank_type));
  }
  TypedColumn<StringPool::Id>* mutable_jank_severity_type() {
    return static_cast<ColumnType::jank_severity_type*>(
        GetColumn(ColumnIndex::jank_severity_type));
  }
  TypedColumn<StringPool::Id>* mutable_prediction_type() {
    return static_cast<ColumnType::prediction_type*>(
        GetColumn(ColumnIndex::prediction_type));
  }
  TypedColumn<StringPool::Id>* mutable_jank_tag() {
    return static_cast<ColumnType::jank_tag*>(
        GetColumn(ColumnIndex::jank_tag));
  }

 private:
  ActualFrameTimelineSliceTable(StringPool* pool,
            const SliceTable& parent,
            const RowMap& parent_overlay,
            ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token
, ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token
, ColumnStorage<ColumnType::upid::stored_type> upid
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name
, ColumnStorage<ColumnType::present_type::stored_type> present_type
, ColumnStorage<ColumnType::on_time_finish::stored_type> on_time_finish
, ColumnStorage<ColumnType::gpu_composition::stored_type> gpu_composition
, ColumnStorage<ColumnType::jank_type::stored_type> jank_type
, ColumnStorage<ColumnType::jank_severity_type::stored_type> jank_severity_type
, ColumnStorage<ColumnType::prediction_type::stored_type> prediction_type
, ColumnStorage<ColumnType::jank_tag::stored_type> jank_tag)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, &parent),
          parent,
          parent_overlay),
          const_parent_(&parent)
,
        display_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::display_frame_token::non_optional_stored_type>(
          &display_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::display_frame_token::stored_type>::ToColumnType(),
          false)),
        surface_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::surface_frame_token::non_optional_stored_type>(
          &surface_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::surface_frame_token::stored_type>::ToColumnType(),
          false)),
        upid_storage_layer_(
        new column::NumericStorage<ColumnType::upid::non_optional_stored_type>(
          &upid_.vector(),
          ColumnTypeHelper<ColumnType::upid::stored_type>::ToColumnType(),
          false)),
        layer_name_storage_layer_(
          new column::StringStorage(string_pool(), &layer_name_.vector())),
        present_type_storage_layer_(
          new column::StringStorage(string_pool(), &present_type_.vector())),
        on_time_finish_storage_layer_(
        new column::NumericStorage<ColumnType::on_time_finish::non_optional_stored_type>(
          &on_time_finish_.vector(),
          ColumnTypeHelper<ColumnType::on_time_finish::stored_type>::ToColumnType(),
          false)),
        gpu_composition_storage_layer_(
        new column::NumericStorage<ColumnType::gpu_composition::non_optional_stored_type>(
          &gpu_composition_.vector(),
          ColumnTypeHelper<ColumnType::gpu_composition::stored_type>::ToColumnType(),
          false)),
        jank_type_storage_layer_(
          new column::StringStorage(string_pool(), &jank_type_.vector())),
        jank_severity_type_storage_layer_(
          new column::StringStorage(string_pool(), &jank_severity_type_.vector())),
        prediction_type_storage_layer_(
          new column::StringStorage(string_pool(), &prediction_type_.vector())),
        jank_tag_storage_layer_(
          new column::StringStorage(string_pool(), &jank_tag_.vector()))
         {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::display_frame_token::stored_type>(
          ColumnFlag::display_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::surface_frame_token::stored_type>(
          ColumnFlag::surface_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::upid::stored_type>(
          ColumnFlag::upid),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::layer_name::stored_type>(
          ColumnFlag::layer_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::present_type::stored_type>(
          ColumnFlag::present_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::on_time_finish::stored_type>(
          ColumnFlag::on_time_finish),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::gpu_composition::stored_type>(
          ColumnFlag::gpu_composition),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::jank_type::stored_type>(
          ColumnFlag::jank_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::jank_severity_type::stored_type>(
          ColumnFlag::jank_severity_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::prediction_type::stored_type>(
          ColumnFlag::prediction_type),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::jank_tag::stored_type>(
          ColumnFlag::jank_tag),
        "Column type and flag combination is not valid");
    PERFETTO_DCHECK(display_frame_token.size() == parent_overlay.size());
    display_frame_token_ = std::move(display_frame_token);
    PERFETTO_DCHECK(surface_frame_token.size() == parent_overlay.size());
    surface_frame_token_ = std::move(surface_frame_token);
    PERFETTO_DCHECK(upid.size() == parent_overlay.size());
    upid_ = std::move(upid);
    PERFETTO_DCHECK(layer_name.size() == parent_overlay.size());
    layer_name_ = std::move(layer_name);
    PERFETTO_DCHECK(present_type.size() == parent_overlay.size());
    present_type_ = std::move(present_type);
    PERFETTO_DCHECK(on_time_finish.size() == parent_overlay.size());
    on_time_finish_ = std::move(on_time_finish);
    PERFETTO_DCHECK(gpu_composition.size() == parent_overlay.size());
    gpu_composition_ = std::move(gpu_composition);
    PERFETTO_DCHECK(jank_type.size() == parent_overlay.size());
    jank_type_ = std::move(jank_type);
    PERFETTO_DCHECK(jank_severity_type.size() == parent_overlay.size());
    jank_severity_type_ = std::move(jank_severity_type);
    PERFETTO_DCHECK(prediction_type.size() == parent_overlay.size());
    prediction_type_ = std::move(prediction_type);
    PERFETTO_DCHECK(jank_tag.size() == parent_overlay.size());
    jank_tag_ = std::move(jank_tag);

    std::vector<RefPtr<column::DataLayer>> overlay_layers(OverlayCount(&parent) + 1);
    for (uint32_t i = 0; i < overlay_layers.size(); ++i) {
      if (overlays()[i].row_map().IsIndexVector()) {
        overlay_layers[i].reset(new column::ArrangementOverlay(
            overlays()[i].row_map().GetIfIndexVector(),
            column::DataLayerChain::Indices::State::kNonmonotonic));
      } else if (overlays()[i].row_map().IsBitVector()) {
        overlay_layers[i].reset(new column::SelectorOverlay(
            overlays()[i].row_map().GetIfBitVector()));
      } else if (overlays()[i].row_map().IsRange()) {
        overlay_layers[i].reset(new column::RangeOverlay(
            overlays()[i].row_map().GetIfIRange()));
      }
    }

    OnConstructionCompleted(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],display_frame_token_storage_layer_,surface_frame_token_storage_layer_,upid_storage_layer_,layer_name_storage_layer_,present_type_storage_layer_,on_time_finish_storage_layer_,gpu_composition_storage_layer_,jank_type_storage_layer_,jank_severity_type_storage_layer_,prediction_type_storage_layer_,jank_tag_storage_layer_}, {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],{},{},{},{},{},{},{},{},{},{},{}}, std::move(overlay_layers));
  }
  SliceTable* parent_ = nullptr;
  const SliceTable* const_parent_ = nullptr;
  ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token_;
  ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token_;
  ColumnStorage<ColumnType::upid::stored_type> upid_;
  ColumnStorage<ColumnType::layer_name::stored_type> layer_name_;
  ColumnStorage<ColumnType::present_type::stored_type> present_type_;
  ColumnStorage<ColumnType::on_time_finish::stored_type> on_time_finish_;
  ColumnStorage<ColumnType::gpu_composition::stored_type> gpu_composition_;
  ColumnStorage<ColumnType::jank_type::stored_type> jank_type_;
  ColumnStorage<ColumnType::jank_severity_type::stored_type> jank_severity_type_;
  ColumnStorage<ColumnType::prediction_type::stored_type> prediction_type_;
  ColumnStorage<ColumnType::jank_tag::stored_type> jank_tag_;

  RefPtr<column::DataLayer> display_frame_token_storage_layer_;
  RefPtr<column::DataLayer> surface_frame_token_storage_layer_;
  RefPtr<column::DataLayer> upid_storage_layer_;
  RefPtr<column::DataLayer> layer_name_storage_layer_;
  RefPtr<column::DataLayer> present_type_storage_layer_;
  RefPtr<column::DataLayer> on_time_finish_storage_layer_;
  RefPtr<column::DataLayer> gpu_composition_storage_layer_;
  RefPtr<column::DataLayer> jank_type_storage_layer_;
  RefPtr<column::DataLayer> jank_severity_type_storage_layer_;
  RefPtr<column::DataLayer> prediction_type_storage_layer_;
  RefPtr<column::DataLayer> jank_tag_storage_layer_;

  
};
  

class ExpectedFrameTimelineSliceTable : public macros_internal::MacroTable {
 public:
  static constexpr uint32_t kColumnCount = 20;

  using Id = SliceTable::Id;
    
  struct ColumnIndex {
    static constexpr uint32_t id = 0;
    static constexpr uint32_t type = 1;
    static constexpr uint32_t ts = 2;
    static constexpr uint32_t dur = 3;
    static constexpr uint32_t track_id = 4;
    static constexpr uint32_t category = 5;
    static constexpr uint32_t name = 6;
    static constexpr uint32_t depth = 7;
    static constexpr uint32_t stack_id = 8;
    static constexpr uint32_t parent_stack_id = 9;
    static constexpr uint32_t parent_id = 10;
    static constexpr uint32_t arg_set_id = 11;
    static constexpr uint32_t thread_ts = 12;
    static constexpr uint32_t thread_dur = 13;
    static constexpr uint32_t thread_instruction_count = 14;
    static constexpr uint32_t thread_instruction_delta = 15;
    static constexpr uint32_t display_frame_token = 16;
    static constexpr uint32_t surface_frame_token = 17;
    static constexpr uint32_t upid = 18;
    static constexpr uint32_t layer_name = 19;
  };
  struct ColumnType {
    using id = IdColumn<ExpectedFrameTimelineSliceTable::Id>;
    using type = TypedColumn<StringPool::Id>;
    using ts = TypedColumn<int64_t>;
    using dur = TypedColumn<int64_t>;
    using track_id = TypedColumn<TrackTable::Id>;
    using category = TypedColumn<std::optional<StringPool::Id>>;
    using name = TypedColumn<std::optional<StringPool::Id>>;
    using depth = TypedColumn<uint32_t>;
    using stack_id = TypedColumn<int64_t>;
    using parent_stack_id = TypedColumn<int64_t>;
    using parent_id = TypedColumn<std::optional<ExpectedFrameTimelineSliceTable::Id>>;
    using arg_set_id = TypedColumn<uint32_t>;
    using thread_ts = TypedColumn<std::optional<int64_t>>;
    using thread_dur = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_count = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_delta = TypedColumn<std::optional<int64_t>>;
    using display_frame_token = TypedColumn<int64_t>;
    using surface_frame_token = TypedColumn<int64_t>;
    using upid = TypedColumn<uint32_t>;
    using layer_name = TypedColumn<StringPool::Id>;
  };
  struct Row : public SliceTable::Row {
    Row(int64_t in_ts = {},
        int64_t in_dur = {},
        TrackTable::Id in_track_id = {},
        std::optional<StringPool::Id> in_category = {},
        std::optional<StringPool::Id> in_name = {},
        uint32_t in_depth = {},
        int64_t in_stack_id = {},
        int64_t in_parent_stack_id = {},
        std::optional<ExpectedFrameTimelineSliceTable::Id> in_parent_id = {},
        uint32_t in_arg_set_id = {},
        std::optional<int64_t> in_thread_ts = {},
        std::optional<int64_t> in_thread_dur = {},
        std::optional<int64_t> in_thread_instruction_count = {},
        std::optional<int64_t> in_thread_instruction_delta = {},
        int64_t in_display_frame_token = {},
        int64_t in_surface_frame_token = {},
        uint32_t in_upid = {},
        StringPool::Id in_layer_name = {},
        std::nullptr_t = nullptr)
        : SliceTable::Row(in_ts, in_dur, in_track_id, in_category, in_name, in_depth, in_stack_id, in_parent_stack_id, in_parent_id, in_arg_set_id, in_thread_ts, in_thread_dur, in_thread_instruction_count, in_thread_instruction_delta),
          display_frame_token(in_display_frame_token),
          surface_frame_token(in_surface_frame_token),
          upid(in_upid),
          layer_name(in_layer_name) {
      type_ = "expected_frame_timeline_slice";
    }
    int64_t display_frame_token;
    int64_t surface_frame_token;
    uint32_t upid;
    StringPool::Id layer_name;

    bool operator==(const ExpectedFrameTimelineSliceTable::Row& other) const {
      return type() == other.type() && ColumnType::ts::Equals(ts, other.ts) &&
       ColumnType::dur::Equals(dur, other.dur) &&
       ColumnType::track_id::Equals(track_id, other.track_id) &&
       ColumnType::category::Equals(category, other.category) &&
       ColumnType::name::Equals(name, other.name) &&
       ColumnType::depth::Equals(depth, other.depth) &&
       ColumnType::stack_id::Equals(stack_id, other.stack_id) &&
       ColumnType::parent_stack_id::Equals(parent_stack_id, other.parent_stack_id) &&
       ColumnType::parent_id::Equals(parent_id, other.parent_id) &&
       ColumnType::arg_set_id::Equals(arg_set_id, other.arg_set_id) &&
       ColumnType::thread_ts::Equals(thread_ts, other.thread_ts) &&
       ColumnType::thread_dur::Equals(thread_dur, other.thread_dur) &&
       ColumnType::thread_instruction_count::Equals(thread_instruction_count, other.thread_instruction_count) &&
       ColumnType::thread_instruction_delta::Equals(thread_instruction_delta, other.thread_instruction_delta) &&
       ColumnType::display_frame_token::Equals(display_frame_token, other.display_frame_token) &&
       ColumnType::surface_frame_token::Equals(surface_frame_token, other.surface_frame_token) &&
       ColumnType::upid::Equals(upid, other.upid) &&
       ColumnType::layer_name::Equals(layer_name, other.layer_name);
    }
  };
  struct ColumnFlag {
    static constexpr uint32_t display_frame_token = ColumnType::display_frame_token::default_flags();
    static constexpr uint32_t surface_frame_token = ColumnType::surface_frame_token::default_flags();
    static constexpr uint32_t upid = ColumnType::upid::default_flags();
    static constexpr uint32_t layer_name = ColumnType::layer_name::default_flags();
  };

  class RowNumber;
  class ConstRowReference;
  class RowReference;

  class RowNumber : public macros_internal::AbstractRowNumber<
      ExpectedFrameTimelineSliceTable, ConstRowReference, RowReference> {
   public:
    explicit RowNumber(uint32_t row_number)
        : AbstractRowNumber(row_number) {}
  };
  static_assert(std::is_trivially_destructible_v<RowNumber>,
                "Inheritance used without trivial destruction");

  class ConstRowReference : public macros_internal::AbstractConstRowReference<
    ExpectedFrameTimelineSliceTable, RowNumber> {
   public:
    ConstRowReference(const ExpectedFrameTimelineSliceTable* table, uint32_t row_number)
        : AbstractConstRowReference(table, row_number) {}

    ColumnType::id::type id() const {
      return table_->id()[row_number_];
    }
    ColumnType::type::type type() const {
      return table_->type()[row_number_];
    }
    ColumnType::ts::type ts() const {
      return table_->ts()[row_number_];
    }
    ColumnType::dur::type dur() const {
      return table_->dur()[row_number_];
    }
    ColumnType::track_id::type track_id() const {
      return table_->track_id()[row_number_];
    }
    ColumnType::category::type category() const {
      return table_->category()[row_number_];
    }
    ColumnType::name::type name() const {
      return table_->name()[row_number_];
    }
    ColumnType::depth::type depth() const {
      return table_->depth()[row_number_];
    }
    ColumnType::stack_id::type stack_id() const {
      return table_->stack_id()[row_number_];
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      return table_->parent_stack_id()[row_number_];
    }
    ColumnType::parent_id::type parent_id() const {
      return table_->parent_id()[row_number_];
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      return table_->arg_set_id()[row_number_];
    }
    ColumnType::thread_ts::type thread_ts() const {
      return table_->thread_ts()[row_number_];
    }
    ColumnType::thread_dur::type thread_dur() const {
      return table_->thread_dur()[row_number_];
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      return table_->thread_instruction_count()[row_number_];
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      return table_->thread_instruction_delta()[row_number_];
    }
    ColumnType::display_frame_token::type display_frame_token() const {
      return table_->display_frame_token()[row_number_];
    }
    ColumnType::surface_frame_token::type surface_frame_token() const {
      return table_->surface_frame_token()[row_number_];
    }
    ColumnType::upid::type upid() const {
      return table_->upid()[row_number_];
    }
    ColumnType::layer_name::type layer_name() const {
      return table_->layer_name()[row_number_];
    }
  };
  static_assert(std::is_trivially_destructible_v<ConstRowReference>,
                "Inheritance used without trivial destruction");
  class RowReference : public ConstRowReference {
   public:
    RowReference(const ExpectedFrameTimelineSliceTable* table, uint32_t row_number)
        : ConstRowReference(table, row_number) {}

    void set_ts(
        ColumnType::ts::non_optional_type v) {
      return mutable_table()->mutable_ts()->Set(row_number_, v);
    }
    void set_dur(
        ColumnType::dur::non_optional_type v) {
      return mutable_table()->mutable_dur()->Set(row_number_, v);
    }
    void set_track_id(
        ColumnType::track_id::non_optional_type v) {
      return mutable_table()->mutable_track_id()->Set(row_number_, v);
    }
    void set_category(
        ColumnType::category::non_optional_type v) {
      return mutable_table()->mutable_category()->Set(row_number_, v);
    }
    void set_name(
        ColumnType::name::non_optional_type v) {
      return mutable_table()->mutable_name()->Set(row_number_, v);
    }
    void set_depth(
        ColumnType::depth::non_optional_type v) {
      return mutable_table()->mutable_depth()->Set(row_number_, v);
    }
    void set_stack_id(
        ColumnType::stack_id::non_optional_type v) {
      return mutable_table()->mutable_stack_id()->Set(row_number_, v);
    }
    void set_parent_stack_id(
        ColumnType::parent_stack_id::non_optional_type v) {
      return mutable_table()->mutable_parent_stack_id()->Set(row_number_, v);
    }
    void set_parent_id(
        ColumnType::parent_id::non_optional_type v) {
      return mutable_table()->mutable_parent_id()->Set(row_number_, v);
    }
    void set_arg_set_id(
        ColumnType::arg_set_id::non_optional_type v) {
      return mutable_table()->mutable_arg_set_id()->Set(row_number_, v);
    }
    void set_thread_ts(
        ColumnType::thread_ts::non_optional_type v) {
      return mutable_table()->mutable_thread_ts()->Set(row_number_, v);
    }
    void set_thread_dur(
        ColumnType::thread_dur::non_optional_type v) {
      return mutable_table()->mutable_thread_dur()->Set(row_number_, v);
    }
    void set_thread_instruction_count(
        ColumnType::thread_instruction_count::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_count()->Set(row_number_, v);
    }
    void set_thread_instruction_delta(
        ColumnType::thread_instruction_delta::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_delta()->Set(row_number_, v);
    }
    void set_display_frame_token(
        ColumnType::display_frame_token::non_optional_type v) {
      return mutable_table()->mutable_display_frame_token()->Set(row_number_, v);
    }
    void set_surface_frame_token(
        ColumnType::surface_frame_token::non_optional_type v) {
      return mutable_table()->mutable_surface_frame_token()->Set(row_number_, v);
    }
    void set_upid(
        ColumnType::upid::non_optional_type v) {
      return mutable_table()->mutable_upid()->Set(row_number_, v);
    }
    void set_layer_name(
        ColumnType::layer_name::non_optional_type v) {
      return mutable_table()->mutable_layer_name()->Set(row_number_, v);
    }

   private:
    ExpectedFrameTimelineSliceTable* mutable_table() const {
      return const_cast<ExpectedFrameTimelineSliceTable*>(table_);
    }
  };
  static_assert(std::is_trivially_destructible_v<RowReference>,
                "Inheritance used without trivial destruction");

  class ConstIterator;
  class ConstIterator : public macros_internal::AbstractConstIterator<
    ConstIterator, ExpectedFrameTimelineSliceTable, RowNumber, ConstRowReference> {
   public:
    ColumnType::id::type id() const {
      const auto& col = table_->id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::type::type type() const {
      const auto& col = table_->type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::ts::type ts() const {
      const auto& col = table_->ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::dur::type dur() const {
      const auto& col = table_->dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::track_id::type track_id() const {
      const auto& col = table_->track_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::category::type category() const {
      const auto& col = table_->category();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::name::type name() const {
      const auto& col = table_->name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::depth::type depth() const {
      const auto& col = table_->depth();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::stack_id::type stack_id() const {
      const auto& col = table_->stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      const auto& col = table_->parent_stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_id::type parent_id() const {
      const auto& col = table_->parent_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      const auto& col = table_->arg_set_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_ts::type thread_ts() const {
      const auto& col = table_->thread_ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_dur::type thread_dur() const {
      const auto& col = table_->thread_dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      const auto& col = table_->thread_instruction_count();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      const auto& col = table_->thread_instruction_delta();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::display_frame_token::type display_frame_token() const {
      const auto& col = table_->display_frame_token();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::surface_frame_token::type surface_frame_token() const {
      const auto& col = table_->surface_frame_token();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::upid::type upid() const {
      const auto& col = table_->upid();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::layer_name::type layer_name() const {
      const auto& col = table_->layer_name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }

   protected:
    explicit ConstIterator(const ExpectedFrameTimelineSliceTable* table,
                           Table::Iterator iterator)
        : AbstractConstIterator(table, std::move(iterator)) {}

    uint32_t CurrentRowNumber() const {
      return iterator_.StorageIndexForLastOverlay();
    }

   private:
    friend class ExpectedFrameTimelineSliceTable;
    friend class macros_internal::AbstractConstIterator<
      ConstIterator, ExpectedFrameTimelineSliceTable, RowNumber, ConstRowReference>;
  };
  class Iterator : public ConstIterator {
    public:
     void set_ts(ColumnType::ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_dur(ColumnType::dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_track_id(ColumnType::track_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_track_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_category(ColumnType::category::non_optional_type v) {
        auto* col = mutable_table_->mutable_category();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_name(ColumnType::name::non_optional_type v) {
        auto* col = mutable_table_->mutable_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_depth(ColumnType::depth::non_optional_type v) {
        auto* col = mutable_table_->mutable_depth();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_stack_id(ColumnType::stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_stack_id(ColumnType::parent_stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_id(ColumnType::parent_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_arg_set_id(ColumnType::arg_set_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_arg_set_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_ts(ColumnType::thread_ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_dur(ColumnType::thread_dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_count(ColumnType::thread_instruction_count::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_count();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_delta(ColumnType::thread_instruction_delta::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_delta();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_display_frame_token(ColumnType::display_frame_token::non_optional_type v) {
        auto* col = mutable_table_->mutable_display_frame_token();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_surface_frame_token(ColumnType::surface_frame_token::non_optional_type v) {
        auto* col = mutable_table_->mutable_surface_frame_token();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_upid(ColumnType::upid::non_optional_type v) {
        auto* col = mutable_table_->mutable_upid();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_layer_name(ColumnType::layer_name::non_optional_type v) {
        auto* col = mutable_table_->mutable_layer_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }

     RowReference row_reference() const {
       return RowReference(mutable_table_, CurrentRowNumber());
     }

    private:
     friend class ExpectedFrameTimelineSliceTable;

     explicit Iterator(ExpectedFrameTimelineSliceTable* table, Table::Iterator iterator)
        : ConstIterator(table, std::move(iterator)),
          mutable_table_(table) {}

     ExpectedFrameTimelineSliceTable* mutable_table_ = nullptr;
  };

  struct IdAndRow {
    Id id;
    uint32_t row;
    RowReference row_reference;
    RowNumber row_number;
  };

  static std::vector<ColumnLegacy> GetColumns(
      ExpectedFrameTimelineSliceTable* self,
      const macros_internal::MacroTable* parent) {
    std::vector<ColumnLegacy> columns =
        CopyColumnsFromParentOrAddRootColumns(self, parent);
    uint32_t olay_idx = OverlayCount(parent);
    AddColumnToVector(columns, "display_frame_token", &self->display_frame_token_, ColumnFlag::display_frame_token,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "surface_frame_token", &self->surface_frame_token_, ColumnFlag::surface_frame_token,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "upid", &self->upid_, ColumnFlag::upid,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "layer_name", &self->layer_name_, ColumnFlag::layer_name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    return columns;
  }

  PERFETTO_NO_INLINE explicit ExpectedFrameTimelineSliceTable(StringPool* pool, SliceTable* parent)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, parent),
          parent),
        parent_(parent), const_parent_(parent), display_frame_token_(ColumnStorage<ColumnType::display_frame_token::stored_type>::Create<false>()),
        surface_frame_token_(ColumnStorage<ColumnType::surface_frame_token::stored_type>::Create<false>()),
        upid_(ColumnStorage<ColumnType::upid::stored_type>::Create<false>()),
        layer_name_(ColumnStorage<ColumnType::layer_name::stored_type>::Create<false>())
,
        display_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::display_frame_token::non_optional_stored_type>(
          &display_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::display_frame_token::stored_type>::ToColumnType(),
          false)),
        surface_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::surface_frame_token::non_optional_stored_type>(
          &surface_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::surface_frame_token::stored_type>::ToColumnType(),
          false)),
        upid_storage_layer_(
        new column::NumericStorage<ColumnType::upid::non_optional_stored_type>(
          &upid_.vector(),
          ColumnTypeHelper<ColumnType::upid::stored_type>::ToColumnType(),
          false)),
        layer_name_storage_layer_(
          new column::StringStorage(string_pool(), &layer_name_.vector()))
         {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::display_frame_token::stored_type>(
          ColumnFlag::display_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::surface_frame_token::stored_type>(
          ColumnFlag::surface_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::upid::stored_type>(
          ColumnFlag::upid),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::layer_name::stored_type>(
          ColumnFlag::layer_name),
        "Column type and flag combination is not valid");
    OnConstructionCompletedRegularConstructor(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],display_frame_token_storage_layer_,surface_frame_token_storage_layer_,upid_storage_layer_,layer_name_storage_layer_},
      {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],{},{},{},{}});
  }
  ~ExpectedFrameTimelineSliceTable() override;

  static const char* Name() { return "expected_frame_timeline_slice"; }

  static Table::Schema ComputeStaticSchema() {
    Table::Schema schema;
    schema.columns.emplace_back(Table::Schema::Column{
        "id", SqlValue::Type::kLong, true, true, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "type", SqlValue::Type::kString, false, false, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "ts", ColumnType::ts::SqlValueType(), false,
        true,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "dur", ColumnType::dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "track_id", ColumnType::track_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "category", ColumnType::category::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "name", ColumnType::name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "depth", ColumnType::depth::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "stack_id", ColumnType::stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_stack_id", ColumnType::parent_stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_id", ColumnType::parent_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "arg_set_id", ColumnType::arg_set_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_ts", ColumnType::thread_ts::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_dur", ColumnType::thread_dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_count", ColumnType::thread_instruction_count::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_delta", ColumnType::thread_instruction_delta::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "display_frame_token", ColumnType::display_frame_token::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "surface_frame_token", ColumnType::surface_frame_token::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "upid", ColumnType::upid::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "layer_name", ColumnType::layer_name::SqlValueType(), false,
        false,
        false,
        false});
    return schema;
  }

  ConstIterator IterateRows() const {
    return ConstIterator(this, Table::IterateRows());
  }

  Iterator IterateRows() { return Iterator(this, Table::IterateRows()); }

  ConstIterator FilterToIterator(
      const std::vector<Constraint>& cs) const {
    return ConstIterator(
      this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  Iterator FilterToIterator(
      const std::vector<Constraint>& cs) {
    return Iterator(this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  void ShrinkToFit() {
    display_frame_token_.ShrinkToFit();
    surface_frame_token_.ShrinkToFit();
    upid_.ShrinkToFit();
    layer_name_.ShrinkToFit();
  }

  std::optional<ConstRowReference> FindById(Id find_id) const {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(ConstRowReference(this, *row))
               : std::nullopt;
  }

  std::optional<RowReference> FindById(Id find_id) {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(RowReference(this, *row)) : std::nullopt;
  }

  IdAndRow Insert(const Row& row) {
    uint32_t row_number = row_count();
    Id id = Id{parent_->Insert(row).id};
    UpdateOverlaysAfterParentInsert();
    mutable_display_frame_token()->Append(row.display_frame_token);
    mutable_surface_frame_token()->Append(row.surface_frame_token);
    mutable_upid()->Append(row.upid);
    mutable_layer_name()->Append(row.layer_name);
    UpdateSelfOverlayAfterInsert();
    return IdAndRow{id, row_number, RowReference(this, row_number),
                     RowNumber(row_number)};
  }

  static std::unique_ptr<Table> ExtendParent(
      const SliceTable& parent,
      ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token
, ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token
, ColumnStorage<ColumnType::upid::stored_type> upid
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name) {
    return std::unique_ptr<Table>(new ExpectedFrameTimelineSliceTable(
        parent.string_pool(), parent, RowMap(0, parent.row_count()),
        std::move(display_frame_token), std::move(surface_frame_token), std::move(upid), std::move(layer_name)));
  }

  static std::unique_ptr<Table> SelectAndExtendParent(
      const SliceTable& parent,
      std::vector<SliceTable::RowNumber> parent_overlay,
      ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token
, ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token
, ColumnStorage<ColumnType::upid::stored_type> upid
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name) {
    std::vector<uint32_t> prs_untyped(parent_overlay.size());
    for (uint32_t i = 0; i < parent_overlay.size(); ++i) {
      prs_untyped[i] = parent_overlay[i].row_number();
    }
    return std::unique_ptr<Table>(new ExpectedFrameTimelineSliceTable(
        parent.string_pool(), parent, RowMap(std::move(prs_untyped)),
        std::move(display_frame_token), std::move(surface_frame_token), std::move(upid), std::move(layer_name)));
  }

  const IdColumn<ExpectedFrameTimelineSliceTable::Id>& id() const {
    return static_cast<const ColumnType::id&>(columns()[ColumnIndex::id]);
  }
  const TypedColumn<StringPool::Id>& type() const {
    return static_cast<const ColumnType::type&>(columns()[ColumnIndex::type]);
  }
  const TypedColumn<int64_t>& ts() const {
    return static_cast<const ColumnType::ts&>(columns()[ColumnIndex::ts]);
  }
  const TypedColumn<int64_t>& dur() const {
    return static_cast<const ColumnType::dur&>(columns()[ColumnIndex::dur]);
  }
  const TypedColumn<TrackTable::Id>& track_id() const {
    return static_cast<const ColumnType::track_id&>(columns()[ColumnIndex::track_id]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& category() const {
    return static_cast<const ColumnType::category&>(columns()[ColumnIndex::category]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& name() const {
    return static_cast<const ColumnType::name&>(columns()[ColumnIndex::name]);
  }
  const TypedColumn<uint32_t>& depth() const {
    return static_cast<const ColumnType::depth&>(columns()[ColumnIndex::depth]);
  }
  const TypedColumn<int64_t>& stack_id() const {
    return static_cast<const ColumnType::stack_id&>(columns()[ColumnIndex::stack_id]);
  }
  const TypedColumn<int64_t>& parent_stack_id() const {
    return static_cast<const ColumnType::parent_stack_id&>(columns()[ColumnIndex::parent_stack_id]);
  }
  const TypedColumn<std::optional<ExpectedFrameTimelineSliceTable::Id>>& parent_id() const {
    return static_cast<const ColumnType::parent_id&>(columns()[ColumnIndex::parent_id]);
  }
  const TypedColumn<uint32_t>& arg_set_id() const {
    return static_cast<const ColumnType::arg_set_id&>(columns()[ColumnIndex::arg_set_id]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_ts() const {
    return static_cast<const ColumnType::thread_ts&>(columns()[ColumnIndex::thread_ts]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_dur() const {
    return static_cast<const ColumnType::thread_dur&>(columns()[ColumnIndex::thread_dur]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_count() const {
    return static_cast<const ColumnType::thread_instruction_count&>(columns()[ColumnIndex::thread_instruction_count]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_delta() const {
    return static_cast<const ColumnType::thread_instruction_delta&>(columns()[ColumnIndex::thread_instruction_delta]);
  }
  const TypedColumn<int64_t>& display_frame_token() const {
    return static_cast<const ColumnType::display_frame_token&>(columns()[ColumnIndex::display_frame_token]);
  }
  const TypedColumn<int64_t>& surface_frame_token() const {
    return static_cast<const ColumnType::surface_frame_token&>(columns()[ColumnIndex::surface_frame_token]);
  }
  const TypedColumn<uint32_t>& upid() const {
    return static_cast<const ColumnType::upid&>(columns()[ColumnIndex::upid]);
  }
  const TypedColumn<StringPool::Id>& layer_name() const {
    return static_cast<const ColumnType::layer_name&>(columns()[ColumnIndex::layer_name]);
  }

  TypedColumn<int64_t>* mutable_ts() {
    return static_cast<ColumnType::ts*>(
        GetColumn(ColumnIndex::ts));
  }
  TypedColumn<int64_t>* mutable_dur() {
    return static_cast<ColumnType::dur*>(
        GetColumn(ColumnIndex::dur));
  }
  TypedColumn<TrackTable::Id>* mutable_track_id() {
    return static_cast<ColumnType::track_id*>(
        GetColumn(ColumnIndex::track_id));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_category() {
    return static_cast<ColumnType::category*>(
        GetColumn(ColumnIndex::category));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_name() {
    return static_cast<ColumnType::name*>(
        GetColumn(ColumnIndex::name));
  }
  TypedColumn<uint32_t>* mutable_depth() {
    return static_cast<ColumnType::depth*>(
        GetColumn(ColumnIndex::depth));
  }
  TypedColumn<int64_t>* mutable_stack_id() {
    return static_cast<ColumnType::stack_id*>(
        GetColumn(ColumnIndex::stack_id));
  }
  TypedColumn<int64_t>* mutable_parent_stack_id() {
    return static_cast<ColumnType::parent_stack_id*>(
        GetColumn(ColumnIndex::parent_stack_id));
  }
  TypedColumn<std::optional<ExpectedFrameTimelineSliceTable::Id>>* mutable_parent_id() {
    return static_cast<ColumnType::parent_id*>(
        GetColumn(ColumnIndex::parent_id));
  }
  TypedColumn<uint32_t>* mutable_arg_set_id() {
    return static_cast<ColumnType::arg_set_id*>(
        GetColumn(ColumnIndex::arg_set_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_ts() {
    return static_cast<ColumnType::thread_ts*>(
        GetColumn(ColumnIndex::thread_ts));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_dur() {
    return static_cast<ColumnType::thread_dur*>(
        GetColumn(ColumnIndex::thread_dur));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_count() {
    return static_cast<ColumnType::thread_instruction_count*>(
        GetColumn(ColumnIndex::thread_instruction_count));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_delta() {
    return static_cast<ColumnType::thread_instruction_delta*>(
        GetColumn(ColumnIndex::thread_instruction_delta));
  }
  TypedColumn<int64_t>* mutable_display_frame_token() {
    return static_cast<ColumnType::display_frame_token*>(
        GetColumn(ColumnIndex::display_frame_token));
  }
  TypedColumn<int64_t>* mutable_surface_frame_token() {
    return static_cast<ColumnType::surface_frame_token*>(
        GetColumn(ColumnIndex::surface_frame_token));
  }
  TypedColumn<uint32_t>* mutable_upid() {
    return static_cast<ColumnType::upid*>(
        GetColumn(ColumnIndex::upid));
  }
  TypedColumn<StringPool::Id>* mutable_layer_name() {
    return static_cast<ColumnType::layer_name*>(
        GetColumn(ColumnIndex::layer_name));
  }

 private:
  ExpectedFrameTimelineSliceTable(StringPool* pool,
            const SliceTable& parent,
            const RowMap& parent_overlay,
            ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token
, ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token
, ColumnStorage<ColumnType::upid::stored_type> upid
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, &parent),
          parent,
          parent_overlay),
          const_parent_(&parent)
,
        display_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::display_frame_token::non_optional_stored_type>(
          &display_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::display_frame_token::stored_type>::ToColumnType(),
          false)),
        surface_frame_token_storage_layer_(
        new column::NumericStorage<ColumnType::surface_frame_token::non_optional_stored_type>(
          &surface_frame_token_.vector(),
          ColumnTypeHelper<ColumnType::surface_frame_token::stored_type>::ToColumnType(),
          false)),
        upid_storage_layer_(
        new column::NumericStorage<ColumnType::upid::non_optional_stored_type>(
          &upid_.vector(),
          ColumnTypeHelper<ColumnType::upid::stored_type>::ToColumnType(),
          false)),
        layer_name_storage_layer_(
          new column::StringStorage(string_pool(), &layer_name_.vector()))
         {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::display_frame_token::stored_type>(
          ColumnFlag::display_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::surface_frame_token::stored_type>(
          ColumnFlag::surface_frame_token),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::upid::stored_type>(
          ColumnFlag::upid),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::layer_name::stored_type>(
          ColumnFlag::layer_name),
        "Column type and flag combination is not valid");
    PERFETTO_DCHECK(display_frame_token.size() == parent_overlay.size());
    display_frame_token_ = std::move(display_frame_token);
    PERFETTO_DCHECK(surface_frame_token.size() == parent_overlay.size());
    surface_frame_token_ = std::move(surface_frame_token);
    PERFETTO_DCHECK(upid.size() == parent_overlay.size());
    upid_ = std::move(upid);
    PERFETTO_DCHECK(layer_name.size() == parent_overlay.size());
    layer_name_ = std::move(layer_name);

    std::vector<RefPtr<column::DataLayer>> overlay_layers(OverlayCount(&parent) + 1);
    for (uint32_t i = 0; i < overlay_layers.size(); ++i) {
      if (overlays()[i].row_map().IsIndexVector()) {
        overlay_layers[i].reset(new column::ArrangementOverlay(
            overlays()[i].row_map().GetIfIndexVector(),
            column::DataLayerChain::Indices::State::kNonmonotonic));
      } else if (overlays()[i].row_map().IsBitVector()) {
        overlay_layers[i].reset(new column::SelectorOverlay(
            overlays()[i].row_map().GetIfBitVector()));
      } else if (overlays()[i].row_map().IsRange()) {
        overlay_layers[i].reset(new column::RangeOverlay(
            overlays()[i].row_map().GetIfIRange()));
      }
    }

    OnConstructionCompleted(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],display_frame_token_storage_layer_,surface_frame_token_storage_layer_,upid_storage_layer_,layer_name_storage_layer_}, {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],{},{},{},{}}, std::move(overlay_layers));
  }
  SliceTable* parent_ = nullptr;
  const SliceTable* const_parent_ = nullptr;
  ColumnStorage<ColumnType::display_frame_token::stored_type> display_frame_token_;
  ColumnStorage<ColumnType::surface_frame_token::stored_type> surface_frame_token_;
  ColumnStorage<ColumnType::upid::stored_type> upid_;
  ColumnStorage<ColumnType::layer_name::stored_type> layer_name_;

  RefPtr<column::DataLayer> display_frame_token_storage_layer_;
  RefPtr<column::DataLayer> surface_frame_token_storage_layer_;
  RefPtr<column::DataLayer> upid_storage_layer_;
  RefPtr<column::DataLayer> layer_name_storage_layer_;

  
};
  

class ExperimentalFlatSliceTable : public macros_internal::MacroTable {
 public:
  static constexpr uint32_t kColumnCount = 11;

  struct Id : public BaseId {
    Id() = default;
    explicit constexpr Id(uint32_t v) : BaseId(v) {}
  };
  static_assert(std::is_trivially_destructible_v<Id>,
                "Inheritance used without trivial destruction");
    
  struct ColumnIndex {
    static constexpr uint32_t id = 0;
    static constexpr uint32_t type = 1;
    static constexpr uint32_t ts = 2;
    static constexpr uint32_t dur = 3;
    static constexpr uint32_t track_id = 4;
    static constexpr uint32_t category = 5;
    static constexpr uint32_t name = 6;
    static constexpr uint32_t arg_set_id = 7;
    static constexpr uint32_t source_id = 8;
    static constexpr uint32_t start_bound = 9;
    static constexpr uint32_t end_bound = 10;
  };
  struct ColumnType {
    using id = IdColumn<ExperimentalFlatSliceTable::Id>;
    using type = TypedColumn<StringPool::Id>;
    using ts = TypedColumn<int64_t>;
    using dur = TypedColumn<int64_t>;
    using track_id = TypedColumn<TrackTable::Id>;
    using category = TypedColumn<std::optional<StringPool::Id>>;
    using name = TypedColumn<std::optional<StringPool::Id>>;
    using arg_set_id = TypedColumn<uint32_t>;
    using source_id = TypedColumn<std::optional<SliceTable::Id>>;
    using start_bound = TypedColumn<int64_t>;
    using end_bound = TypedColumn<int64_t>;
  };
  struct Row : public macros_internal::RootParentTable::Row {
    Row(int64_t in_ts = {},
        int64_t in_dur = {},
        TrackTable::Id in_track_id = {},
        std::optional<StringPool::Id> in_category = {},
        std::optional<StringPool::Id> in_name = {},
        uint32_t in_arg_set_id = {},
        std::optional<SliceTable::Id> in_source_id = {},
        int64_t in_start_bound = {},
        int64_t in_end_bound = {},
        std::nullptr_t = nullptr)
        : macros_internal::RootParentTable::Row(),
          ts(in_ts),
          dur(in_dur),
          track_id(in_track_id),
          category(in_category),
          name(in_name),
          arg_set_id(in_arg_set_id),
          source_id(in_source_id),
          start_bound(in_start_bound),
          end_bound(in_end_bound) {
      type_ = "experimental_flat_slice";
    }
    int64_t ts;
    int64_t dur;
    TrackTable::Id track_id;
    std::optional<StringPool::Id> category;
    std::optional<StringPool::Id> name;
    uint32_t arg_set_id;
    std::optional<SliceTable::Id> source_id;
    int64_t start_bound;
    int64_t end_bound;

    bool operator==(const ExperimentalFlatSliceTable::Row& other) const {
      return type() == other.type() && ColumnType::ts::Equals(ts, other.ts) &&
       ColumnType::dur::Equals(dur, other.dur) &&
       ColumnType::track_id::Equals(track_id, other.track_id) &&
       ColumnType::category::Equals(category, other.category) &&
       ColumnType::name::Equals(name, other.name) &&
       ColumnType::arg_set_id::Equals(arg_set_id, other.arg_set_id) &&
       ColumnType::source_id::Equals(source_id, other.source_id) &&
       ColumnType::start_bound::Equals(start_bound, other.start_bound) &&
       ColumnType::end_bound::Equals(end_bound, other.end_bound);
    }
  };
  struct ColumnFlag {
    static constexpr uint32_t ts = ColumnType::ts::default_flags();
    static constexpr uint32_t dur = ColumnType::dur::default_flags();
    static constexpr uint32_t track_id = ColumnType::track_id::default_flags();
    static constexpr uint32_t category = ColumnType::category::default_flags();
    static constexpr uint32_t name = ColumnType::name::default_flags();
    static constexpr uint32_t arg_set_id = ColumnType::arg_set_id::default_flags();
    static constexpr uint32_t source_id = ColumnType::source_id::default_flags();
    static constexpr uint32_t start_bound = static_cast<uint32_t>(ColumnLegacy::Flag::kHidden) | ColumnType::start_bound::default_flags();
    static constexpr uint32_t end_bound = static_cast<uint32_t>(ColumnLegacy::Flag::kHidden) | ColumnType::end_bound::default_flags();
  };

  class RowNumber;
  class ConstRowReference;
  class RowReference;

  class RowNumber : public macros_internal::AbstractRowNumber<
      ExperimentalFlatSliceTable, ConstRowReference, RowReference> {
   public:
    explicit RowNumber(uint32_t row_number)
        : AbstractRowNumber(row_number) {}
  };
  static_assert(std::is_trivially_destructible_v<RowNumber>,
                "Inheritance used without trivial destruction");

  class ConstRowReference : public macros_internal::AbstractConstRowReference<
    ExperimentalFlatSliceTable, RowNumber> {
   public:
    ConstRowReference(const ExperimentalFlatSliceTable* table, uint32_t row_number)
        : AbstractConstRowReference(table, row_number) {}

    ColumnType::id::type id() const {
      return table_->id()[row_number_];
    }
    ColumnType::type::type type() const {
      return table_->type()[row_number_];
    }
    ColumnType::ts::type ts() const {
      return table_->ts()[row_number_];
    }
    ColumnType::dur::type dur() const {
      return table_->dur()[row_number_];
    }
    ColumnType::track_id::type track_id() const {
      return table_->track_id()[row_number_];
    }
    ColumnType::category::type category() const {
      return table_->category()[row_number_];
    }
    ColumnType::name::type name() const {
      return table_->name()[row_number_];
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      return table_->arg_set_id()[row_number_];
    }
    ColumnType::source_id::type source_id() const {
      return table_->source_id()[row_number_];
    }
    ColumnType::start_bound::type start_bound() const {
      return table_->start_bound()[row_number_];
    }
    ColumnType::end_bound::type end_bound() const {
      return table_->end_bound()[row_number_];
    }
  };
  static_assert(std::is_trivially_destructible_v<ConstRowReference>,
                "Inheritance used without trivial destruction");
  class RowReference : public ConstRowReference {
   public:
    RowReference(const ExperimentalFlatSliceTable* table, uint32_t row_number)
        : ConstRowReference(table, row_number) {}

    void set_ts(
        ColumnType::ts::non_optional_type v) {
      return mutable_table()->mutable_ts()->Set(row_number_, v);
    }
    void set_dur(
        ColumnType::dur::non_optional_type v) {
      return mutable_table()->mutable_dur()->Set(row_number_, v);
    }
    void set_track_id(
        ColumnType::track_id::non_optional_type v) {
      return mutable_table()->mutable_track_id()->Set(row_number_, v);
    }
    void set_category(
        ColumnType::category::non_optional_type v) {
      return mutable_table()->mutable_category()->Set(row_number_, v);
    }
    void set_name(
        ColumnType::name::non_optional_type v) {
      return mutable_table()->mutable_name()->Set(row_number_, v);
    }
    void set_arg_set_id(
        ColumnType::arg_set_id::non_optional_type v) {
      return mutable_table()->mutable_arg_set_id()->Set(row_number_, v);
    }
    void set_source_id(
        ColumnType::source_id::non_optional_type v) {
      return mutable_table()->mutable_source_id()->Set(row_number_, v);
    }
    void set_start_bound(
        ColumnType::start_bound::non_optional_type v) {
      return mutable_table()->mutable_start_bound()->Set(row_number_, v);
    }
    void set_end_bound(
        ColumnType::end_bound::non_optional_type v) {
      return mutable_table()->mutable_end_bound()->Set(row_number_, v);
    }

   private:
    ExperimentalFlatSliceTable* mutable_table() const {
      return const_cast<ExperimentalFlatSliceTable*>(table_);
    }
  };
  static_assert(std::is_trivially_destructible_v<RowReference>,
                "Inheritance used without trivial destruction");

  class ConstIterator;
  class ConstIterator : public macros_internal::AbstractConstIterator<
    ConstIterator, ExperimentalFlatSliceTable, RowNumber, ConstRowReference> {
   public:
    ColumnType::id::type id() const {
      const auto& col = table_->id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::type::type type() const {
      const auto& col = table_->type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::ts::type ts() const {
      const auto& col = table_->ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::dur::type dur() const {
      const auto& col = table_->dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::track_id::type track_id() const {
      const auto& col = table_->track_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::category::type category() const {
      const auto& col = table_->category();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::name::type name() const {
      const auto& col = table_->name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      const auto& col = table_->arg_set_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::source_id::type source_id() const {
      const auto& col = table_->source_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::start_bound::type start_bound() const {
      const auto& col = table_->start_bound();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::end_bound::type end_bound() const {
      const auto& col = table_->end_bound();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }

   protected:
    explicit ConstIterator(const ExperimentalFlatSliceTable* table,
                           Table::Iterator iterator)
        : AbstractConstIterator(table, std::move(iterator)) {}

    uint32_t CurrentRowNumber() const {
      return iterator_.StorageIndexForLastOverlay();
    }

   private:
    friend class ExperimentalFlatSliceTable;
    friend class macros_internal::AbstractConstIterator<
      ConstIterator, ExperimentalFlatSliceTable, RowNumber, ConstRowReference>;
  };
  class Iterator : public ConstIterator {
    public:
     void set_ts(ColumnType::ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_dur(ColumnType::dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_track_id(ColumnType::track_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_track_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_category(ColumnType::category::non_optional_type v) {
        auto* col = mutable_table_->mutable_category();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_name(ColumnType::name::non_optional_type v) {
        auto* col = mutable_table_->mutable_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_arg_set_id(ColumnType::arg_set_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_arg_set_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_source_id(ColumnType::source_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_source_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_start_bound(ColumnType::start_bound::non_optional_type v) {
        auto* col = mutable_table_->mutable_start_bound();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_end_bound(ColumnType::end_bound::non_optional_type v) {
        auto* col = mutable_table_->mutable_end_bound();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }

     RowReference row_reference() const {
       return RowReference(mutable_table_, CurrentRowNumber());
     }

    private:
     friend class ExperimentalFlatSliceTable;

     explicit Iterator(ExperimentalFlatSliceTable* table, Table::Iterator iterator)
        : ConstIterator(table, std::move(iterator)),
          mutable_table_(table) {}

     ExperimentalFlatSliceTable* mutable_table_ = nullptr;
  };

  struct IdAndRow {
    Id id;
    uint32_t row;
    RowReference row_reference;
    RowNumber row_number;
  };

  static std::vector<ColumnLegacy> GetColumns(
      ExperimentalFlatSliceTable* self,
      const macros_internal::MacroTable* parent) {
    std::vector<ColumnLegacy> columns =
        CopyColumnsFromParentOrAddRootColumns(self, parent);
    uint32_t olay_idx = OverlayCount(parent);
    AddColumnToVector(columns, "ts", &self->ts_, ColumnFlag::ts,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "dur", &self->dur_, ColumnFlag::dur,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "track_id", &self->track_id_, ColumnFlag::track_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "category", &self->category_, ColumnFlag::category,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "name", &self->name_, ColumnFlag::name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "arg_set_id", &self->arg_set_id_, ColumnFlag::arg_set_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "source_id", &self->source_id_, ColumnFlag::source_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "start_bound", &self->start_bound_, ColumnFlag::start_bound,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "end_bound", &self->end_bound_, ColumnFlag::end_bound,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    return columns;
  }

  PERFETTO_NO_INLINE explicit ExperimentalFlatSliceTable(StringPool* pool)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, nullptr),
          nullptr),
        ts_(ColumnStorage<ColumnType::ts::stored_type>::Create<false>()),
        dur_(ColumnStorage<ColumnType::dur::stored_type>::Create<false>()),
        track_id_(ColumnStorage<ColumnType::track_id::stored_type>::Create<false>()),
        category_(ColumnStorage<ColumnType::category::stored_type>::Create<false>()),
        name_(ColumnStorage<ColumnType::name::stored_type>::Create<false>()),
        arg_set_id_(ColumnStorage<ColumnType::arg_set_id::stored_type>::Create<false>()),
        source_id_(ColumnStorage<ColumnType::source_id::stored_type>::Create<false>()),
        start_bound_(ColumnStorage<ColumnType::start_bound::stored_type>::Create<false>()),
        end_bound_(ColumnStorage<ColumnType::end_bound::stored_type>::Create<false>())
,
        id_storage_layer_(new column::IdStorage()),
        type_storage_layer_(
          new column::StringStorage(string_pool(), &type_.vector())),
        ts_storage_layer_(
        new column::NumericStorage<ColumnType::ts::non_optional_stored_type>(
          &ts_.vector(),
          ColumnTypeHelper<ColumnType::ts::stored_type>::ToColumnType(),
          false)),
        dur_storage_layer_(
        new column::NumericStorage<ColumnType::dur::non_optional_stored_type>(
          &dur_.vector(),
          ColumnTypeHelper<ColumnType::dur::stored_type>::ToColumnType(),
          false)),
        track_id_storage_layer_(
        new column::NumericStorage<ColumnType::track_id::non_optional_stored_type>(
          &track_id_.vector(),
          ColumnTypeHelper<ColumnType::track_id::stored_type>::ToColumnType(),
          false)),
        category_storage_layer_(
          new column::StringStorage(string_pool(), &category_.vector())),
        name_storage_layer_(
          new column::StringStorage(string_pool(), &name_.vector())),
        arg_set_id_storage_layer_(
        new column::NumericStorage<ColumnType::arg_set_id::non_optional_stored_type>(
          &arg_set_id_.vector(),
          ColumnTypeHelper<ColumnType::arg_set_id::stored_type>::ToColumnType(),
          false)),
        source_id_storage_layer_(
          new column::NumericStorage<ColumnType::source_id::non_optional_stored_type>(
            &source_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::source_id::stored_type>::ToColumnType(),
            false)),
        start_bound_storage_layer_(
        new column::NumericStorage<ColumnType::start_bound::non_optional_stored_type>(
          &start_bound_.vector(),
          ColumnTypeHelper<ColumnType::start_bound::stored_type>::ToColumnType(),
          false)),
        end_bound_storage_layer_(
        new column::NumericStorage<ColumnType::end_bound::non_optional_stored_type>(
          &end_bound_.vector(),
          ColumnTypeHelper<ColumnType::end_bound::stored_type>::ToColumnType(),
          false))
,
        source_id_null_layer_(new column::NullOverlay(source_id_.bv())) {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::ts::stored_type>(
          ColumnFlag::ts),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::dur::stored_type>(
          ColumnFlag::dur),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::track_id::stored_type>(
          ColumnFlag::track_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::category::stored_type>(
          ColumnFlag::category),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::name::stored_type>(
          ColumnFlag::name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::arg_set_id::stored_type>(
          ColumnFlag::arg_set_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::source_id::stored_type>(
          ColumnFlag::source_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::start_bound::stored_type>(
          ColumnFlag::start_bound),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::end_bound::stored_type>(
          ColumnFlag::end_bound),
        "Column type and flag combination is not valid");
    OnConstructionCompletedRegularConstructor(
      {id_storage_layer_,type_storage_layer_,ts_storage_layer_,dur_storage_layer_,track_id_storage_layer_,category_storage_layer_,name_storage_layer_,arg_set_id_storage_layer_,source_id_storage_layer_,start_bound_storage_layer_,end_bound_storage_layer_},
      {{},{},{},{},{},{},{},{},source_id_null_layer_,{},{}});
  }
  ~ExperimentalFlatSliceTable() override;

  static const char* Name() { return "experimental_flat_slice"; }

  static Table::Schema ComputeStaticSchema() {
    Table::Schema schema;
    schema.columns.emplace_back(Table::Schema::Column{
        "id", SqlValue::Type::kLong, true, true, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "type", SqlValue::Type::kString, false, false, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "ts", ColumnType::ts::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "dur", ColumnType::dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "track_id", ColumnType::track_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "category", ColumnType::category::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "name", ColumnType::name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "arg_set_id", ColumnType::arg_set_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "source_id", ColumnType::source_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "start_bound", ColumnType::start_bound::SqlValueType(), false,
        false,
        true,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "end_bound", ColumnType::end_bound::SqlValueType(), false,
        false,
        true,
        false});
    return schema;
  }

  ConstIterator IterateRows() const {
    return ConstIterator(this, Table::IterateRows());
  }

  Iterator IterateRows() { return Iterator(this, Table::IterateRows()); }

  ConstIterator FilterToIterator(
      const std::vector<Constraint>& cs) const {
    return ConstIterator(
      this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  Iterator FilterToIterator(
      const std::vector<Constraint>& cs) {
    return Iterator(this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  void ShrinkToFit() {
    type_.ShrinkToFit();
    ts_.ShrinkToFit();
    dur_.ShrinkToFit();
    track_id_.ShrinkToFit();
    category_.ShrinkToFit();
    name_.ShrinkToFit();
    arg_set_id_.ShrinkToFit();
    source_id_.ShrinkToFit();
    start_bound_.ShrinkToFit();
    end_bound_.ShrinkToFit();
  }

  std::optional<ConstRowReference> FindById(Id find_id) const {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(ConstRowReference(this, *row))
               : std::nullopt;
  }

  std::optional<RowReference> FindById(Id find_id) {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(RowReference(this, *row)) : std::nullopt;
  }

  IdAndRow Insert(const Row& row) {
    uint32_t row_number = row_count();
    Id id = Id{row_number};
    type_.Append(string_pool()->InternString(row.type()));
    mutable_ts()->Append(row.ts);
    mutable_dur()->Append(row.dur);
    mutable_track_id()->Append(row.track_id);
    mutable_category()->Append(row.category);
    mutable_name()->Append(row.name);
    mutable_arg_set_id()->Append(row.arg_set_id);
    mutable_source_id()->Append(row.source_id);
    mutable_start_bound()->Append(row.start_bound);
    mutable_end_bound()->Append(row.end_bound);
    UpdateSelfOverlayAfterInsert();
    return IdAndRow{id, row_number, RowReference(this, row_number),
                     RowNumber(row_number)};
  }

  

  const IdColumn<ExperimentalFlatSliceTable::Id>& id() const {
    return static_cast<const ColumnType::id&>(columns()[ColumnIndex::id]);
  }
  const TypedColumn<StringPool::Id>& type() const {
    return static_cast<const ColumnType::type&>(columns()[ColumnIndex::type]);
  }
  const TypedColumn<int64_t>& ts() const {
    return static_cast<const ColumnType::ts&>(columns()[ColumnIndex::ts]);
  }
  const TypedColumn<int64_t>& dur() const {
    return static_cast<const ColumnType::dur&>(columns()[ColumnIndex::dur]);
  }
  const TypedColumn<TrackTable::Id>& track_id() const {
    return static_cast<const ColumnType::track_id&>(columns()[ColumnIndex::track_id]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& category() const {
    return static_cast<const ColumnType::category&>(columns()[ColumnIndex::category]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& name() const {
    return static_cast<const ColumnType::name&>(columns()[ColumnIndex::name]);
  }
  const TypedColumn<uint32_t>& arg_set_id() const {
    return static_cast<const ColumnType::arg_set_id&>(columns()[ColumnIndex::arg_set_id]);
  }
  const TypedColumn<std::optional<SliceTable::Id>>& source_id() const {
    return static_cast<const ColumnType::source_id&>(columns()[ColumnIndex::source_id]);
  }
  const TypedColumn<int64_t>& start_bound() const {
    return static_cast<const ColumnType::start_bound&>(columns()[ColumnIndex::start_bound]);
  }
  const TypedColumn<int64_t>& end_bound() const {
    return static_cast<const ColumnType::end_bound&>(columns()[ColumnIndex::end_bound]);
  }

  TypedColumn<int64_t>* mutable_ts() {
    return static_cast<ColumnType::ts*>(
        GetColumn(ColumnIndex::ts));
  }
  TypedColumn<int64_t>* mutable_dur() {
    return static_cast<ColumnType::dur*>(
        GetColumn(ColumnIndex::dur));
  }
  TypedColumn<TrackTable::Id>* mutable_track_id() {
    return static_cast<ColumnType::track_id*>(
        GetColumn(ColumnIndex::track_id));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_category() {
    return static_cast<ColumnType::category*>(
        GetColumn(ColumnIndex::category));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_name() {
    return static_cast<ColumnType::name*>(
        GetColumn(ColumnIndex::name));
  }
  TypedColumn<uint32_t>* mutable_arg_set_id() {
    return static_cast<ColumnType::arg_set_id*>(
        GetColumn(ColumnIndex::arg_set_id));
  }
  TypedColumn<std::optional<SliceTable::Id>>* mutable_source_id() {
    return static_cast<ColumnType::source_id*>(
        GetColumn(ColumnIndex::source_id));
  }
  TypedColumn<int64_t>* mutable_start_bound() {
    return static_cast<ColumnType::start_bound*>(
        GetColumn(ColumnIndex::start_bound));
  }
  TypedColumn<int64_t>* mutable_end_bound() {
    return static_cast<ColumnType::end_bound*>(
        GetColumn(ColumnIndex::end_bound));
  }

 private:
  
  
  ColumnStorage<ColumnType::ts::stored_type> ts_;
  ColumnStorage<ColumnType::dur::stored_type> dur_;
  ColumnStorage<ColumnType::track_id::stored_type> track_id_;
  ColumnStorage<ColumnType::category::stored_type> category_;
  ColumnStorage<ColumnType::name::stored_type> name_;
  ColumnStorage<ColumnType::arg_set_id::stored_type> arg_set_id_;
  ColumnStorage<ColumnType::source_id::stored_type> source_id_;
  ColumnStorage<ColumnType::start_bound::stored_type> start_bound_;
  ColumnStorage<ColumnType::end_bound::stored_type> end_bound_;

  RefPtr<column::DataLayer> id_storage_layer_;
  RefPtr<column::DataLayer> type_storage_layer_;
  RefPtr<column::DataLayer> ts_storage_layer_;
  RefPtr<column::DataLayer> dur_storage_layer_;
  RefPtr<column::DataLayer> track_id_storage_layer_;
  RefPtr<column::DataLayer> category_storage_layer_;
  RefPtr<column::DataLayer> name_storage_layer_;
  RefPtr<column::DataLayer> arg_set_id_storage_layer_;
  RefPtr<column::DataLayer> source_id_storage_layer_;
  RefPtr<column::DataLayer> start_bound_storage_layer_;
  RefPtr<column::DataLayer> end_bound_storage_layer_;

  RefPtr<column::DataLayer> source_id_null_layer_;
};
  

class GpuSliceTable : public macros_internal::MacroTable {
 public:
  static constexpr uint32_t kColumnCount = 27;

  using Id = SliceTable::Id;
    
  struct ColumnIndex {
    static constexpr uint32_t id = 0;
    static constexpr uint32_t type = 1;
    static constexpr uint32_t ts = 2;
    static constexpr uint32_t dur = 3;
    static constexpr uint32_t track_id = 4;
    static constexpr uint32_t category = 5;
    static constexpr uint32_t name = 6;
    static constexpr uint32_t depth = 7;
    static constexpr uint32_t stack_id = 8;
    static constexpr uint32_t parent_stack_id = 9;
    static constexpr uint32_t parent_id = 10;
    static constexpr uint32_t arg_set_id = 11;
    static constexpr uint32_t thread_ts = 12;
    static constexpr uint32_t thread_dur = 13;
    static constexpr uint32_t thread_instruction_count = 14;
    static constexpr uint32_t thread_instruction_delta = 15;
    static constexpr uint32_t context_id = 16;
    static constexpr uint32_t render_target = 17;
    static constexpr uint32_t render_target_name = 18;
    static constexpr uint32_t render_pass = 19;
    static constexpr uint32_t render_pass_name = 20;
    static constexpr uint32_t command_buffer = 21;
    static constexpr uint32_t command_buffer_name = 22;
    static constexpr uint32_t frame_id = 23;
    static constexpr uint32_t submission_id = 24;
    static constexpr uint32_t hw_queue_id = 25;
    static constexpr uint32_t render_subpasses = 26;
  };
  struct ColumnType {
    using id = IdColumn<GpuSliceTable::Id>;
    using type = TypedColumn<StringPool::Id>;
    using ts = TypedColumn<int64_t>;
    using dur = TypedColumn<int64_t>;
    using track_id = TypedColumn<TrackTable::Id>;
    using category = TypedColumn<std::optional<StringPool::Id>>;
    using name = TypedColumn<std::optional<StringPool::Id>>;
    using depth = TypedColumn<uint32_t>;
    using stack_id = TypedColumn<int64_t>;
    using parent_stack_id = TypedColumn<int64_t>;
    using parent_id = TypedColumn<std::optional<GpuSliceTable::Id>>;
    using arg_set_id = TypedColumn<uint32_t>;
    using thread_ts = TypedColumn<std::optional<int64_t>>;
    using thread_dur = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_count = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_delta = TypedColumn<std::optional<int64_t>>;
    using context_id = TypedColumn<std::optional<int64_t>>;
    using render_target = TypedColumn<std::optional<int64_t>>;
    using render_target_name = TypedColumn<StringPool::Id>;
    using render_pass = TypedColumn<std::optional<int64_t>>;
    using render_pass_name = TypedColumn<StringPool::Id>;
    using command_buffer = TypedColumn<std::optional<int64_t>>;
    using command_buffer_name = TypedColumn<StringPool::Id>;
    using frame_id = TypedColumn<std::optional<uint32_t>>;
    using submission_id = TypedColumn<std::optional<uint32_t>>;
    using hw_queue_id = TypedColumn<std::optional<int64_t>>;
    using render_subpasses = TypedColumn<StringPool::Id>;
  };
  struct Row : public SliceTable::Row {
    Row(int64_t in_ts = {},
        int64_t in_dur = {},
        TrackTable::Id in_track_id = {},
        std::optional<StringPool::Id> in_category = {},
        std::optional<StringPool::Id> in_name = {},
        uint32_t in_depth = {},
        int64_t in_stack_id = {},
        int64_t in_parent_stack_id = {},
        std::optional<GpuSliceTable::Id> in_parent_id = {},
        uint32_t in_arg_set_id = {},
        std::optional<int64_t> in_thread_ts = {},
        std::optional<int64_t> in_thread_dur = {},
        std::optional<int64_t> in_thread_instruction_count = {},
        std::optional<int64_t> in_thread_instruction_delta = {},
        std::optional<int64_t> in_context_id = {},
        std::optional<int64_t> in_render_target = {},
        StringPool::Id in_render_target_name = {},
        std::optional<int64_t> in_render_pass = {},
        StringPool::Id in_render_pass_name = {},
        std::optional<int64_t> in_command_buffer = {},
        StringPool::Id in_command_buffer_name = {},
        std::optional<uint32_t> in_frame_id = {},
        std::optional<uint32_t> in_submission_id = {},
        std::optional<int64_t> in_hw_queue_id = {},
        StringPool::Id in_render_subpasses = {},
        std::nullptr_t = nullptr)
        : SliceTable::Row(in_ts, in_dur, in_track_id, in_category, in_name, in_depth, in_stack_id, in_parent_stack_id, in_parent_id, in_arg_set_id, in_thread_ts, in_thread_dur, in_thread_instruction_count, in_thread_instruction_delta),
          context_id(in_context_id),
          render_target(in_render_target),
          render_target_name(in_render_target_name),
          render_pass(in_render_pass),
          render_pass_name(in_render_pass_name),
          command_buffer(in_command_buffer),
          command_buffer_name(in_command_buffer_name),
          frame_id(in_frame_id),
          submission_id(in_submission_id),
          hw_queue_id(in_hw_queue_id),
          render_subpasses(in_render_subpasses) {
      type_ = "gpu_slice";
    }
    std::optional<int64_t> context_id;
    std::optional<int64_t> render_target;
    StringPool::Id render_target_name;
    std::optional<int64_t> render_pass;
    StringPool::Id render_pass_name;
    std::optional<int64_t> command_buffer;
    StringPool::Id command_buffer_name;
    std::optional<uint32_t> frame_id;
    std::optional<uint32_t> submission_id;
    std::optional<int64_t> hw_queue_id;
    StringPool::Id render_subpasses;

    bool operator==(const GpuSliceTable::Row& other) const {
      return type() == other.type() && ColumnType::ts::Equals(ts, other.ts) &&
       ColumnType::dur::Equals(dur, other.dur) &&
       ColumnType::track_id::Equals(track_id, other.track_id) &&
       ColumnType::category::Equals(category, other.category) &&
       ColumnType::name::Equals(name, other.name) &&
       ColumnType::depth::Equals(depth, other.depth) &&
       ColumnType::stack_id::Equals(stack_id, other.stack_id) &&
       ColumnType::parent_stack_id::Equals(parent_stack_id, other.parent_stack_id) &&
       ColumnType::parent_id::Equals(parent_id, other.parent_id) &&
       ColumnType::arg_set_id::Equals(arg_set_id, other.arg_set_id) &&
       ColumnType::thread_ts::Equals(thread_ts, other.thread_ts) &&
       ColumnType::thread_dur::Equals(thread_dur, other.thread_dur) &&
       ColumnType::thread_instruction_count::Equals(thread_instruction_count, other.thread_instruction_count) &&
       ColumnType::thread_instruction_delta::Equals(thread_instruction_delta, other.thread_instruction_delta) &&
       ColumnType::context_id::Equals(context_id, other.context_id) &&
       ColumnType::render_target::Equals(render_target, other.render_target) &&
       ColumnType::render_target_name::Equals(render_target_name, other.render_target_name) &&
       ColumnType::render_pass::Equals(render_pass, other.render_pass) &&
       ColumnType::render_pass_name::Equals(render_pass_name, other.render_pass_name) &&
       ColumnType::command_buffer::Equals(command_buffer, other.command_buffer) &&
       ColumnType::command_buffer_name::Equals(command_buffer_name, other.command_buffer_name) &&
       ColumnType::frame_id::Equals(frame_id, other.frame_id) &&
       ColumnType::submission_id::Equals(submission_id, other.submission_id) &&
       ColumnType::hw_queue_id::Equals(hw_queue_id, other.hw_queue_id) &&
       ColumnType::render_subpasses::Equals(render_subpasses, other.render_subpasses);
    }
  };
  struct ColumnFlag {
    static constexpr uint32_t context_id = ColumnType::context_id::default_flags();
    static constexpr uint32_t render_target = ColumnType::render_target::default_flags();
    static constexpr uint32_t render_target_name = ColumnType::render_target_name::default_flags();
    static constexpr uint32_t render_pass = ColumnType::render_pass::default_flags();
    static constexpr uint32_t render_pass_name = ColumnType::render_pass_name::default_flags();
    static constexpr uint32_t command_buffer = ColumnType::command_buffer::default_flags();
    static constexpr uint32_t command_buffer_name = ColumnType::command_buffer_name::default_flags();
    static constexpr uint32_t frame_id = ColumnType::frame_id::default_flags();
    static constexpr uint32_t submission_id = ColumnType::submission_id::default_flags();
    static constexpr uint32_t hw_queue_id = ColumnType::hw_queue_id::default_flags();
    static constexpr uint32_t render_subpasses = ColumnType::render_subpasses::default_flags();
  };

  class RowNumber;
  class ConstRowReference;
  class RowReference;

  class RowNumber : public macros_internal::AbstractRowNumber<
      GpuSliceTable, ConstRowReference, RowReference> {
   public:
    explicit RowNumber(uint32_t row_number)
        : AbstractRowNumber(row_number) {}
  };
  static_assert(std::is_trivially_destructible_v<RowNumber>,
                "Inheritance used without trivial destruction");

  class ConstRowReference : public macros_internal::AbstractConstRowReference<
    GpuSliceTable, RowNumber> {
   public:
    ConstRowReference(const GpuSliceTable* table, uint32_t row_number)
        : AbstractConstRowReference(table, row_number) {}

    ColumnType::id::type id() const {
      return table_->id()[row_number_];
    }
    ColumnType::type::type type() const {
      return table_->type()[row_number_];
    }
    ColumnType::ts::type ts() const {
      return table_->ts()[row_number_];
    }
    ColumnType::dur::type dur() const {
      return table_->dur()[row_number_];
    }
    ColumnType::track_id::type track_id() const {
      return table_->track_id()[row_number_];
    }
    ColumnType::category::type category() const {
      return table_->category()[row_number_];
    }
    ColumnType::name::type name() const {
      return table_->name()[row_number_];
    }
    ColumnType::depth::type depth() const {
      return table_->depth()[row_number_];
    }
    ColumnType::stack_id::type stack_id() const {
      return table_->stack_id()[row_number_];
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      return table_->parent_stack_id()[row_number_];
    }
    ColumnType::parent_id::type parent_id() const {
      return table_->parent_id()[row_number_];
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      return table_->arg_set_id()[row_number_];
    }
    ColumnType::thread_ts::type thread_ts() const {
      return table_->thread_ts()[row_number_];
    }
    ColumnType::thread_dur::type thread_dur() const {
      return table_->thread_dur()[row_number_];
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      return table_->thread_instruction_count()[row_number_];
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      return table_->thread_instruction_delta()[row_number_];
    }
    ColumnType::context_id::type context_id() const {
      return table_->context_id()[row_number_];
    }
    ColumnType::render_target::type render_target() const {
      return table_->render_target()[row_number_];
    }
    ColumnType::render_target_name::type render_target_name() const {
      return table_->render_target_name()[row_number_];
    }
    ColumnType::render_pass::type render_pass() const {
      return table_->render_pass()[row_number_];
    }
    ColumnType::render_pass_name::type render_pass_name() const {
      return table_->render_pass_name()[row_number_];
    }
    ColumnType::command_buffer::type command_buffer() const {
      return table_->command_buffer()[row_number_];
    }
    ColumnType::command_buffer_name::type command_buffer_name() const {
      return table_->command_buffer_name()[row_number_];
    }
    ColumnType::frame_id::type frame_id() const {
      return table_->frame_id()[row_number_];
    }
    ColumnType::submission_id::type submission_id() const {
      return table_->submission_id()[row_number_];
    }
    ColumnType::hw_queue_id::type hw_queue_id() const {
      return table_->hw_queue_id()[row_number_];
    }
    ColumnType::render_subpasses::type render_subpasses() const {
      return table_->render_subpasses()[row_number_];
    }
  };
  static_assert(std::is_trivially_destructible_v<ConstRowReference>,
                "Inheritance used without trivial destruction");
  class RowReference : public ConstRowReference {
   public:
    RowReference(const GpuSliceTable* table, uint32_t row_number)
        : ConstRowReference(table, row_number) {}

    void set_ts(
        ColumnType::ts::non_optional_type v) {
      return mutable_table()->mutable_ts()->Set(row_number_, v);
    }
    void set_dur(
        ColumnType::dur::non_optional_type v) {
      return mutable_table()->mutable_dur()->Set(row_number_, v);
    }
    void set_track_id(
        ColumnType::track_id::non_optional_type v) {
      return mutable_table()->mutable_track_id()->Set(row_number_, v);
    }
    void set_category(
        ColumnType::category::non_optional_type v) {
      return mutable_table()->mutable_category()->Set(row_number_, v);
    }
    void set_name(
        ColumnType::name::non_optional_type v) {
      return mutable_table()->mutable_name()->Set(row_number_, v);
    }
    void set_depth(
        ColumnType::depth::non_optional_type v) {
      return mutable_table()->mutable_depth()->Set(row_number_, v);
    }
    void set_stack_id(
        ColumnType::stack_id::non_optional_type v) {
      return mutable_table()->mutable_stack_id()->Set(row_number_, v);
    }
    void set_parent_stack_id(
        ColumnType::parent_stack_id::non_optional_type v) {
      return mutable_table()->mutable_parent_stack_id()->Set(row_number_, v);
    }
    void set_parent_id(
        ColumnType::parent_id::non_optional_type v) {
      return mutable_table()->mutable_parent_id()->Set(row_number_, v);
    }
    void set_arg_set_id(
        ColumnType::arg_set_id::non_optional_type v) {
      return mutable_table()->mutable_arg_set_id()->Set(row_number_, v);
    }
    void set_thread_ts(
        ColumnType::thread_ts::non_optional_type v) {
      return mutable_table()->mutable_thread_ts()->Set(row_number_, v);
    }
    void set_thread_dur(
        ColumnType::thread_dur::non_optional_type v) {
      return mutable_table()->mutable_thread_dur()->Set(row_number_, v);
    }
    void set_thread_instruction_count(
        ColumnType::thread_instruction_count::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_count()->Set(row_number_, v);
    }
    void set_thread_instruction_delta(
        ColumnType::thread_instruction_delta::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_delta()->Set(row_number_, v);
    }
    void set_context_id(
        ColumnType::context_id::non_optional_type v) {
      return mutable_table()->mutable_context_id()->Set(row_number_, v);
    }
    void set_render_target(
        ColumnType::render_target::non_optional_type v) {
      return mutable_table()->mutable_render_target()->Set(row_number_, v);
    }
    void set_render_target_name(
        ColumnType::render_target_name::non_optional_type v) {
      return mutable_table()->mutable_render_target_name()->Set(row_number_, v);
    }
    void set_render_pass(
        ColumnType::render_pass::non_optional_type v) {
      return mutable_table()->mutable_render_pass()->Set(row_number_, v);
    }
    void set_render_pass_name(
        ColumnType::render_pass_name::non_optional_type v) {
      return mutable_table()->mutable_render_pass_name()->Set(row_number_, v);
    }
    void set_command_buffer(
        ColumnType::command_buffer::non_optional_type v) {
      return mutable_table()->mutable_command_buffer()->Set(row_number_, v);
    }
    void set_command_buffer_name(
        ColumnType::command_buffer_name::non_optional_type v) {
      return mutable_table()->mutable_command_buffer_name()->Set(row_number_, v);
    }
    void set_frame_id(
        ColumnType::frame_id::non_optional_type v) {
      return mutable_table()->mutable_frame_id()->Set(row_number_, v);
    }
    void set_submission_id(
        ColumnType::submission_id::non_optional_type v) {
      return mutable_table()->mutable_submission_id()->Set(row_number_, v);
    }
    void set_hw_queue_id(
        ColumnType::hw_queue_id::non_optional_type v) {
      return mutable_table()->mutable_hw_queue_id()->Set(row_number_, v);
    }
    void set_render_subpasses(
        ColumnType::render_subpasses::non_optional_type v) {
      return mutable_table()->mutable_render_subpasses()->Set(row_number_, v);
    }

   private:
    GpuSliceTable* mutable_table() const {
      return const_cast<GpuSliceTable*>(table_);
    }
  };
  static_assert(std::is_trivially_destructible_v<RowReference>,
                "Inheritance used without trivial destruction");

  class ConstIterator;
  class ConstIterator : public macros_internal::AbstractConstIterator<
    ConstIterator, GpuSliceTable, RowNumber, ConstRowReference> {
   public:
    ColumnType::id::type id() const {
      const auto& col = table_->id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::type::type type() const {
      const auto& col = table_->type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::ts::type ts() const {
      const auto& col = table_->ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::dur::type dur() const {
      const auto& col = table_->dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::track_id::type track_id() const {
      const auto& col = table_->track_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::category::type category() const {
      const auto& col = table_->category();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::name::type name() const {
      const auto& col = table_->name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::depth::type depth() const {
      const auto& col = table_->depth();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::stack_id::type stack_id() const {
      const auto& col = table_->stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      const auto& col = table_->parent_stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_id::type parent_id() const {
      const auto& col = table_->parent_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      const auto& col = table_->arg_set_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_ts::type thread_ts() const {
      const auto& col = table_->thread_ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_dur::type thread_dur() const {
      const auto& col = table_->thread_dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      const auto& col = table_->thread_instruction_count();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      const auto& col = table_->thread_instruction_delta();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::context_id::type context_id() const {
      const auto& col = table_->context_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::render_target::type render_target() const {
      const auto& col = table_->render_target();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::render_target_name::type render_target_name() const {
      const auto& col = table_->render_target_name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::render_pass::type render_pass() const {
      const auto& col = table_->render_pass();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::render_pass_name::type render_pass_name() const {
      const auto& col = table_->render_pass_name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::command_buffer::type command_buffer() const {
      const auto& col = table_->command_buffer();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::command_buffer_name::type command_buffer_name() const {
      const auto& col = table_->command_buffer_name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::frame_id::type frame_id() const {
      const auto& col = table_->frame_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::submission_id::type submission_id() const {
      const auto& col = table_->submission_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::hw_queue_id::type hw_queue_id() const {
      const auto& col = table_->hw_queue_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::render_subpasses::type render_subpasses() const {
      const auto& col = table_->render_subpasses();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }

   protected:
    explicit ConstIterator(const GpuSliceTable* table,
                           Table::Iterator iterator)
        : AbstractConstIterator(table, std::move(iterator)) {}

    uint32_t CurrentRowNumber() const {
      return iterator_.StorageIndexForLastOverlay();
    }

   private:
    friend class GpuSliceTable;
    friend class macros_internal::AbstractConstIterator<
      ConstIterator, GpuSliceTable, RowNumber, ConstRowReference>;
  };
  class Iterator : public ConstIterator {
    public:
     void set_ts(ColumnType::ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_dur(ColumnType::dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_track_id(ColumnType::track_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_track_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_category(ColumnType::category::non_optional_type v) {
        auto* col = mutable_table_->mutable_category();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_name(ColumnType::name::non_optional_type v) {
        auto* col = mutable_table_->mutable_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_depth(ColumnType::depth::non_optional_type v) {
        auto* col = mutable_table_->mutable_depth();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_stack_id(ColumnType::stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_stack_id(ColumnType::parent_stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_id(ColumnType::parent_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_arg_set_id(ColumnType::arg_set_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_arg_set_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_ts(ColumnType::thread_ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_dur(ColumnType::thread_dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_count(ColumnType::thread_instruction_count::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_count();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_delta(ColumnType::thread_instruction_delta::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_delta();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_context_id(ColumnType::context_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_context_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_render_target(ColumnType::render_target::non_optional_type v) {
        auto* col = mutable_table_->mutable_render_target();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_render_target_name(ColumnType::render_target_name::non_optional_type v) {
        auto* col = mutable_table_->mutable_render_target_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_render_pass(ColumnType::render_pass::non_optional_type v) {
        auto* col = mutable_table_->mutable_render_pass();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_render_pass_name(ColumnType::render_pass_name::non_optional_type v) {
        auto* col = mutable_table_->mutable_render_pass_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_command_buffer(ColumnType::command_buffer::non_optional_type v) {
        auto* col = mutable_table_->mutable_command_buffer();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_command_buffer_name(ColumnType::command_buffer_name::non_optional_type v) {
        auto* col = mutable_table_->mutable_command_buffer_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_frame_id(ColumnType::frame_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_frame_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_submission_id(ColumnType::submission_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_submission_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_hw_queue_id(ColumnType::hw_queue_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_hw_queue_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_render_subpasses(ColumnType::render_subpasses::non_optional_type v) {
        auto* col = mutable_table_->mutable_render_subpasses();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }

     RowReference row_reference() const {
       return RowReference(mutable_table_, CurrentRowNumber());
     }

    private:
     friend class GpuSliceTable;

     explicit Iterator(GpuSliceTable* table, Table::Iterator iterator)
        : ConstIterator(table, std::move(iterator)),
          mutable_table_(table) {}

     GpuSliceTable* mutable_table_ = nullptr;
  };

  struct IdAndRow {
    Id id;
    uint32_t row;
    RowReference row_reference;
    RowNumber row_number;
  };

  static std::vector<ColumnLegacy> GetColumns(
      GpuSliceTable* self,
      const macros_internal::MacroTable* parent) {
    std::vector<ColumnLegacy> columns =
        CopyColumnsFromParentOrAddRootColumns(self, parent);
    uint32_t olay_idx = OverlayCount(parent);
    AddColumnToVector(columns, "context_id", &self->context_id_, ColumnFlag::context_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "render_target", &self->render_target_, ColumnFlag::render_target,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "render_target_name", &self->render_target_name_, ColumnFlag::render_target_name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "render_pass", &self->render_pass_, ColumnFlag::render_pass,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "render_pass_name", &self->render_pass_name_, ColumnFlag::render_pass_name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "command_buffer", &self->command_buffer_, ColumnFlag::command_buffer,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "command_buffer_name", &self->command_buffer_name_, ColumnFlag::command_buffer_name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "frame_id", &self->frame_id_, ColumnFlag::frame_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "submission_id", &self->submission_id_, ColumnFlag::submission_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "hw_queue_id", &self->hw_queue_id_, ColumnFlag::hw_queue_id,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "render_subpasses", &self->render_subpasses_, ColumnFlag::render_subpasses,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    return columns;
  }

  PERFETTO_NO_INLINE explicit GpuSliceTable(StringPool* pool, SliceTable* parent)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, parent),
          parent),
        parent_(parent), const_parent_(parent), context_id_(ColumnStorage<ColumnType::context_id::stored_type>::Create<false>()),
        render_target_(ColumnStorage<ColumnType::render_target::stored_type>::Create<false>()),
        render_target_name_(ColumnStorage<ColumnType::render_target_name::stored_type>::Create<false>()),
        render_pass_(ColumnStorage<ColumnType::render_pass::stored_type>::Create<false>()),
        render_pass_name_(ColumnStorage<ColumnType::render_pass_name::stored_type>::Create<false>()),
        command_buffer_(ColumnStorage<ColumnType::command_buffer::stored_type>::Create<false>()),
        command_buffer_name_(ColumnStorage<ColumnType::command_buffer_name::stored_type>::Create<false>()),
        frame_id_(ColumnStorage<ColumnType::frame_id::stored_type>::Create<false>()),
        submission_id_(ColumnStorage<ColumnType::submission_id::stored_type>::Create<false>()),
        hw_queue_id_(ColumnStorage<ColumnType::hw_queue_id::stored_type>::Create<false>()),
        render_subpasses_(ColumnStorage<ColumnType::render_subpasses::stored_type>::Create<false>())
,
        context_id_storage_layer_(
          new column::NumericStorage<ColumnType::context_id::non_optional_stored_type>(
            &context_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::context_id::stored_type>::ToColumnType(),
            false)),
        render_target_storage_layer_(
          new column::NumericStorage<ColumnType::render_target::non_optional_stored_type>(
            &render_target_.non_null_vector(),
            ColumnTypeHelper<ColumnType::render_target::stored_type>::ToColumnType(),
            false)),
        render_target_name_storage_layer_(
          new column::StringStorage(string_pool(), &render_target_name_.vector())),
        render_pass_storage_layer_(
          new column::NumericStorage<ColumnType::render_pass::non_optional_stored_type>(
            &render_pass_.non_null_vector(),
            ColumnTypeHelper<ColumnType::render_pass::stored_type>::ToColumnType(),
            false)),
        render_pass_name_storage_layer_(
          new column::StringStorage(string_pool(), &render_pass_name_.vector())),
        command_buffer_storage_layer_(
          new column::NumericStorage<ColumnType::command_buffer::non_optional_stored_type>(
            &command_buffer_.non_null_vector(),
            ColumnTypeHelper<ColumnType::command_buffer::stored_type>::ToColumnType(),
            false)),
        command_buffer_name_storage_layer_(
          new column::StringStorage(string_pool(), &command_buffer_name_.vector())),
        frame_id_storage_layer_(
          new column::NumericStorage<ColumnType::frame_id::non_optional_stored_type>(
            &frame_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::frame_id::stored_type>::ToColumnType(),
            false)),
        submission_id_storage_layer_(
          new column::NumericStorage<ColumnType::submission_id::non_optional_stored_type>(
            &submission_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::submission_id::stored_type>::ToColumnType(),
            false)),
        hw_queue_id_storage_layer_(
          new column::NumericStorage<ColumnType::hw_queue_id::non_optional_stored_type>(
            &hw_queue_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::hw_queue_id::stored_type>::ToColumnType(),
            false)),
        render_subpasses_storage_layer_(
          new column::StringStorage(string_pool(), &render_subpasses_.vector()))
,
        context_id_null_layer_(new column::NullOverlay(context_id_.bv())),
        render_target_null_layer_(new column::NullOverlay(render_target_.bv())),
        render_pass_null_layer_(new column::NullOverlay(render_pass_.bv())),
        command_buffer_null_layer_(new column::NullOverlay(command_buffer_.bv())),
        frame_id_null_layer_(new column::NullOverlay(frame_id_.bv())),
        submission_id_null_layer_(new column::NullOverlay(submission_id_.bv())),
        hw_queue_id_null_layer_(new column::NullOverlay(hw_queue_id_.bv())) {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::context_id::stored_type>(
          ColumnFlag::context_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_target::stored_type>(
          ColumnFlag::render_target),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_target_name::stored_type>(
          ColumnFlag::render_target_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_pass::stored_type>(
          ColumnFlag::render_pass),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_pass_name::stored_type>(
          ColumnFlag::render_pass_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::command_buffer::stored_type>(
          ColumnFlag::command_buffer),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::command_buffer_name::stored_type>(
          ColumnFlag::command_buffer_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::frame_id::stored_type>(
          ColumnFlag::frame_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::submission_id::stored_type>(
          ColumnFlag::submission_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::hw_queue_id::stored_type>(
          ColumnFlag::hw_queue_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_subpasses::stored_type>(
          ColumnFlag::render_subpasses),
        "Column type and flag combination is not valid");
    OnConstructionCompletedRegularConstructor(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],context_id_storage_layer_,render_target_storage_layer_,render_target_name_storage_layer_,render_pass_storage_layer_,render_pass_name_storage_layer_,command_buffer_storage_layer_,command_buffer_name_storage_layer_,frame_id_storage_layer_,submission_id_storage_layer_,hw_queue_id_storage_layer_,render_subpasses_storage_layer_},
      {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],context_id_null_layer_,render_target_null_layer_,{},render_pass_null_layer_,{},command_buffer_null_layer_,{},frame_id_null_layer_,submission_id_null_layer_,hw_queue_id_null_layer_,{}});
  }
  ~GpuSliceTable() override;

  static const char* Name() { return "gpu_slice"; }

  static Table::Schema ComputeStaticSchema() {
    Table::Schema schema;
    schema.columns.emplace_back(Table::Schema::Column{
        "id", SqlValue::Type::kLong, true, true, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "type", SqlValue::Type::kString, false, false, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "ts", ColumnType::ts::SqlValueType(), false,
        true,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "dur", ColumnType::dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "track_id", ColumnType::track_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "category", ColumnType::category::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "name", ColumnType::name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "depth", ColumnType::depth::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "stack_id", ColumnType::stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_stack_id", ColumnType::parent_stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_id", ColumnType::parent_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "arg_set_id", ColumnType::arg_set_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_ts", ColumnType::thread_ts::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_dur", ColumnType::thread_dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_count", ColumnType::thread_instruction_count::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_delta", ColumnType::thread_instruction_delta::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "context_id", ColumnType::context_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "render_target", ColumnType::render_target::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "render_target_name", ColumnType::render_target_name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "render_pass", ColumnType::render_pass::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "render_pass_name", ColumnType::render_pass_name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "command_buffer", ColumnType::command_buffer::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "command_buffer_name", ColumnType::command_buffer_name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "frame_id", ColumnType::frame_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "submission_id", ColumnType::submission_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "hw_queue_id", ColumnType::hw_queue_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "render_subpasses", ColumnType::render_subpasses::SqlValueType(), false,
        false,
        false,
        false});
    return schema;
  }

  ConstIterator IterateRows() const {
    return ConstIterator(this, Table::IterateRows());
  }

  Iterator IterateRows() { return Iterator(this, Table::IterateRows()); }

  ConstIterator FilterToIterator(
      const std::vector<Constraint>& cs) const {
    return ConstIterator(
      this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  Iterator FilterToIterator(
      const std::vector<Constraint>& cs) {
    return Iterator(this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  void ShrinkToFit() {
    context_id_.ShrinkToFit();
    render_target_.ShrinkToFit();
    render_target_name_.ShrinkToFit();
    render_pass_.ShrinkToFit();
    render_pass_name_.ShrinkToFit();
    command_buffer_.ShrinkToFit();
    command_buffer_name_.ShrinkToFit();
    frame_id_.ShrinkToFit();
    submission_id_.ShrinkToFit();
    hw_queue_id_.ShrinkToFit();
    render_subpasses_.ShrinkToFit();
  }

  std::optional<ConstRowReference> FindById(Id find_id) const {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(ConstRowReference(this, *row))
               : std::nullopt;
  }

  std::optional<RowReference> FindById(Id find_id) {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(RowReference(this, *row)) : std::nullopt;
  }

  IdAndRow Insert(const Row& row) {
    uint32_t row_number = row_count();
    Id id = Id{parent_->Insert(row).id};
    UpdateOverlaysAfterParentInsert();
    mutable_context_id()->Append(row.context_id);
    mutable_render_target()->Append(row.render_target);
    mutable_render_target_name()->Append(row.render_target_name);
    mutable_render_pass()->Append(row.render_pass);
    mutable_render_pass_name()->Append(row.render_pass_name);
    mutable_command_buffer()->Append(row.command_buffer);
    mutable_command_buffer_name()->Append(row.command_buffer_name);
    mutable_frame_id()->Append(row.frame_id);
    mutable_submission_id()->Append(row.submission_id);
    mutable_hw_queue_id()->Append(row.hw_queue_id);
    mutable_render_subpasses()->Append(row.render_subpasses);
    UpdateSelfOverlayAfterInsert();
    return IdAndRow{id, row_number, RowReference(this, row_number),
                     RowNumber(row_number)};
  }

  static std::unique_ptr<Table> ExtendParent(
      const SliceTable& parent,
      ColumnStorage<ColumnType::context_id::stored_type> context_id
, ColumnStorage<ColumnType::render_target::stored_type> render_target
, ColumnStorage<ColumnType::render_target_name::stored_type> render_target_name
, ColumnStorage<ColumnType::render_pass::stored_type> render_pass
, ColumnStorage<ColumnType::render_pass_name::stored_type> render_pass_name
, ColumnStorage<ColumnType::command_buffer::stored_type> command_buffer
, ColumnStorage<ColumnType::command_buffer_name::stored_type> command_buffer_name
, ColumnStorage<ColumnType::frame_id::stored_type> frame_id
, ColumnStorage<ColumnType::submission_id::stored_type> submission_id
, ColumnStorage<ColumnType::hw_queue_id::stored_type> hw_queue_id
, ColumnStorage<ColumnType::render_subpasses::stored_type> render_subpasses) {
    return std::unique_ptr<Table>(new GpuSliceTable(
        parent.string_pool(), parent, RowMap(0, parent.row_count()),
        std::move(context_id), std::move(render_target), std::move(render_target_name), std::move(render_pass), std::move(render_pass_name), std::move(command_buffer), std::move(command_buffer_name), std::move(frame_id), std::move(submission_id), std::move(hw_queue_id), std::move(render_subpasses)));
  }

  static std::unique_ptr<Table> SelectAndExtendParent(
      const SliceTable& parent,
      std::vector<SliceTable::RowNumber> parent_overlay,
      ColumnStorage<ColumnType::context_id::stored_type> context_id
, ColumnStorage<ColumnType::render_target::stored_type> render_target
, ColumnStorage<ColumnType::render_target_name::stored_type> render_target_name
, ColumnStorage<ColumnType::render_pass::stored_type> render_pass
, ColumnStorage<ColumnType::render_pass_name::stored_type> render_pass_name
, ColumnStorage<ColumnType::command_buffer::stored_type> command_buffer
, ColumnStorage<ColumnType::command_buffer_name::stored_type> command_buffer_name
, ColumnStorage<ColumnType::frame_id::stored_type> frame_id
, ColumnStorage<ColumnType::submission_id::stored_type> submission_id
, ColumnStorage<ColumnType::hw_queue_id::stored_type> hw_queue_id
, ColumnStorage<ColumnType::render_subpasses::stored_type> render_subpasses) {
    std::vector<uint32_t> prs_untyped(parent_overlay.size());
    for (uint32_t i = 0; i < parent_overlay.size(); ++i) {
      prs_untyped[i] = parent_overlay[i].row_number();
    }
    return std::unique_ptr<Table>(new GpuSliceTable(
        parent.string_pool(), parent, RowMap(std::move(prs_untyped)),
        std::move(context_id), std::move(render_target), std::move(render_target_name), std::move(render_pass), std::move(render_pass_name), std::move(command_buffer), std::move(command_buffer_name), std::move(frame_id), std::move(submission_id), std::move(hw_queue_id), std::move(render_subpasses)));
  }

  const IdColumn<GpuSliceTable::Id>& id() const {
    return static_cast<const ColumnType::id&>(columns()[ColumnIndex::id]);
  }
  const TypedColumn<StringPool::Id>& type() const {
    return static_cast<const ColumnType::type&>(columns()[ColumnIndex::type]);
  }
  const TypedColumn<int64_t>& ts() const {
    return static_cast<const ColumnType::ts&>(columns()[ColumnIndex::ts]);
  }
  const TypedColumn<int64_t>& dur() const {
    return static_cast<const ColumnType::dur&>(columns()[ColumnIndex::dur]);
  }
  const TypedColumn<TrackTable::Id>& track_id() const {
    return static_cast<const ColumnType::track_id&>(columns()[ColumnIndex::track_id]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& category() const {
    return static_cast<const ColumnType::category&>(columns()[ColumnIndex::category]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& name() const {
    return static_cast<const ColumnType::name&>(columns()[ColumnIndex::name]);
  }
  const TypedColumn<uint32_t>& depth() const {
    return static_cast<const ColumnType::depth&>(columns()[ColumnIndex::depth]);
  }
  const TypedColumn<int64_t>& stack_id() const {
    return static_cast<const ColumnType::stack_id&>(columns()[ColumnIndex::stack_id]);
  }
  const TypedColumn<int64_t>& parent_stack_id() const {
    return static_cast<const ColumnType::parent_stack_id&>(columns()[ColumnIndex::parent_stack_id]);
  }
  const TypedColumn<std::optional<GpuSliceTable::Id>>& parent_id() const {
    return static_cast<const ColumnType::parent_id&>(columns()[ColumnIndex::parent_id]);
  }
  const TypedColumn<uint32_t>& arg_set_id() const {
    return static_cast<const ColumnType::arg_set_id&>(columns()[ColumnIndex::arg_set_id]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_ts() const {
    return static_cast<const ColumnType::thread_ts&>(columns()[ColumnIndex::thread_ts]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_dur() const {
    return static_cast<const ColumnType::thread_dur&>(columns()[ColumnIndex::thread_dur]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_count() const {
    return static_cast<const ColumnType::thread_instruction_count&>(columns()[ColumnIndex::thread_instruction_count]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_delta() const {
    return static_cast<const ColumnType::thread_instruction_delta&>(columns()[ColumnIndex::thread_instruction_delta]);
  }
  const TypedColumn<std::optional<int64_t>>& context_id() const {
    return static_cast<const ColumnType::context_id&>(columns()[ColumnIndex::context_id]);
  }
  const TypedColumn<std::optional<int64_t>>& render_target() const {
    return static_cast<const ColumnType::render_target&>(columns()[ColumnIndex::render_target]);
  }
  const TypedColumn<StringPool::Id>& render_target_name() const {
    return static_cast<const ColumnType::render_target_name&>(columns()[ColumnIndex::render_target_name]);
  }
  const TypedColumn<std::optional<int64_t>>& render_pass() const {
    return static_cast<const ColumnType::render_pass&>(columns()[ColumnIndex::render_pass]);
  }
  const TypedColumn<StringPool::Id>& render_pass_name() const {
    return static_cast<const ColumnType::render_pass_name&>(columns()[ColumnIndex::render_pass_name]);
  }
  const TypedColumn<std::optional<int64_t>>& command_buffer() const {
    return static_cast<const ColumnType::command_buffer&>(columns()[ColumnIndex::command_buffer]);
  }
  const TypedColumn<StringPool::Id>& command_buffer_name() const {
    return static_cast<const ColumnType::command_buffer_name&>(columns()[ColumnIndex::command_buffer_name]);
  }
  const TypedColumn<std::optional<uint32_t>>& frame_id() const {
    return static_cast<const ColumnType::frame_id&>(columns()[ColumnIndex::frame_id]);
  }
  const TypedColumn<std::optional<uint32_t>>& submission_id() const {
    return static_cast<const ColumnType::submission_id&>(columns()[ColumnIndex::submission_id]);
  }
  const TypedColumn<std::optional<int64_t>>& hw_queue_id() const {
    return static_cast<const ColumnType::hw_queue_id&>(columns()[ColumnIndex::hw_queue_id]);
  }
  const TypedColumn<StringPool::Id>& render_subpasses() const {
    return static_cast<const ColumnType::render_subpasses&>(columns()[ColumnIndex::render_subpasses]);
  }

  TypedColumn<int64_t>* mutable_ts() {
    return static_cast<ColumnType::ts*>(
        GetColumn(ColumnIndex::ts));
  }
  TypedColumn<int64_t>* mutable_dur() {
    return static_cast<ColumnType::dur*>(
        GetColumn(ColumnIndex::dur));
  }
  TypedColumn<TrackTable::Id>* mutable_track_id() {
    return static_cast<ColumnType::track_id*>(
        GetColumn(ColumnIndex::track_id));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_category() {
    return static_cast<ColumnType::category*>(
        GetColumn(ColumnIndex::category));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_name() {
    return static_cast<ColumnType::name*>(
        GetColumn(ColumnIndex::name));
  }
  TypedColumn<uint32_t>* mutable_depth() {
    return static_cast<ColumnType::depth*>(
        GetColumn(ColumnIndex::depth));
  }
  TypedColumn<int64_t>* mutable_stack_id() {
    return static_cast<ColumnType::stack_id*>(
        GetColumn(ColumnIndex::stack_id));
  }
  TypedColumn<int64_t>* mutable_parent_stack_id() {
    return static_cast<ColumnType::parent_stack_id*>(
        GetColumn(ColumnIndex::parent_stack_id));
  }
  TypedColumn<std::optional<GpuSliceTable::Id>>* mutable_parent_id() {
    return static_cast<ColumnType::parent_id*>(
        GetColumn(ColumnIndex::parent_id));
  }
  TypedColumn<uint32_t>* mutable_arg_set_id() {
    return static_cast<ColumnType::arg_set_id*>(
        GetColumn(ColumnIndex::arg_set_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_ts() {
    return static_cast<ColumnType::thread_ts*>(
        GetColumn(ColumnIndex::thread_ts));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_dur() {
    return static_cast<ColumnType::thread_dur*>(
        GetColumn(ColumnIndex::thread_dur));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_count() {
    return static_cast<ColumnType::thread_instruction_count*>(
        GetColumn(ColumnIndex::thread_instruction_count));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_delta() {
    return static_cast<ColumnType::thread_instruction_delta*>(
        GetColumn(ColumnIndex::thread_instruction_delta));
  }
  TypedColumn<std::optional<int64_t>>* mutable_context_id() {
    return static_cast<ColumnType::context_id*>(
        GetColumn(ColumnIndex::context_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_render_target() {
    return static_cast<ColumnType::render_target*>(
        GetColumn(ColumnIndex::render_target));
  }
  TypedColumn<StringPool::Id>* mutable_render_target_name() {
    return static_cast<ColumnType::render_target_name*>(
        GetColumn(ColumnIndex::render_target_name));
  }
  TypedColumn<std::optional<int64_t>>* mutable_render_pass() {
    return static_cast<ColumnType::render_pass*>(
        GetColumn(ColumnIndex::render_pass));
  }
  TypedColumn<StringPool::Id>* mutable_render_pass_name() {
    return static_cast<ColumnType::render_pass_name*>(
        GetColumn(ColumnIndex::render_pass_name));
  }
  TypedColumn<std::optional<int64_t>>* mutable_command_buffer() {
    return static_cast<ColumnType::command_buffer*>(
        GetColumn(ColumnIndex::command_buffer));
  }
  TypedColumn<StringPool::Id>* mutable_command_buffer_name() {
    return static_cast<ColumnType::command_buffer_name*>(
        GetColumn(ColumnIndex::command_buffer_name));
  }
  TypedColumn<std::optional<uint32_t>>* mutable_frame_id() {
    return static_cast<ColumnType::frame_id*>(
        GetColumn(ColumnIndex::frame_id));
  }
  TypedColumn<std::optional<uint32_t>>* mutable_submission_id() {
    return static_cast<ColumnType::submission_id*>(
        GetColumn(ColumnIndex::submission_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_hw_queue_id() {
    return static_cast<ColumnType::hw_queue_id*>(
        GetColumn(ColumnIndex::hw_queue_id));
  }
  TypedColumn<StringPool::Id>* mutable_render_subpasses() {
    return static_cast<ColumnType::render_subpasses*>(
        GetColumn(ColumnIndex::render_subpasses));
  }

 private:
  GpuSliceTable(StringPool* pool,
            const SliceTable& parent,
            const RowMap& parent_overlay,
            ColumnStorage<ColumnType::context_id::stored_type> context_id
, ColumnStorage<ColumnType::render_target::stored_type> render_target
, ColumnStorage<ColumnType::render_target_name::stored_type> render_target_name
, ColumnStorage<ColumnType::render_pass::stored_type> render_pass
, ColumnStorage<ColumnType::render_pass_name::stored_type> render_pass_name
, ColumnStorage<ColumnType::command_buffer::stored_type> command_buffer
, ColumnStorage<ColumnType::command_buffer_name::stored_type> command_buffer_name
, ColumnStorage<ColumnType::frame_id::stored_type> frame_id
, ColumnStorage<ColumnType::submission_id::stored_type> submission_id
, ColumnStorage<ColumnType::hw_queue_id::stored_type> hw_queue_id
, ColumnStorage<ColumnType::render_subpasses::stored_type> render_subpasses)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, &parent),
          parent,
          parent_overlay),
          const_parent_(&parent)
,
        context_id_storage_layer_(
          new column::NumericStorage<ColumnType::context_id::non_optional_stored_type>(
            &context_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::context_id::stored_type>::ToColumnType(),
            false)),
        render_target_storage_layer_(
          new column::NumericStorage<ColumnType::render_target::non_optional_stored_type>(
            &render_target_.non_null_vector(),
            ColumnTypeHelper<ColumnType::render_target::stored_type>::ToColumnType(),
            false)),
        render_target_name_storage_layer_(
          new column::StringStorage(string_pool(), &render_target_name_.vector())),
        render_pass_storage_layer_(
          new column::NumericStorage<ColumnType::render_pass::non_optional_stored_type>(
            &render_pass_.non_null_vector(),
            ColumnTypeHelper<ColumnType::render_pass::stored_type>::ToColumnType(),
            false)),
        render_pass_name_storage_layer_(
          new column::StringStorage(string_pool(), &render_pass_name_.vector())),
        command_buffer_storage_layer_(
          new column::NumericStorage<ColumnType::command_buffer::non_optional_stored_type>(
            &command_buffer_.non_null_vector(),
            ColumnTypeHelper<ColumnType::command_buffer::stored_type>::ToColumnType(),
            false)),
        command_buffer_name_storage_layer_(
          new column::StringStorage(string_pool(), &command_buffer_name_.vector())),
        frame_id_storage_layer_(
          new column::NumericStorage<ColumnType::frame_id::non_optional_stored_type>(
            &frame_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::frame_id::stored_type>::ToColumnType(),
            false)),
        submission_id_storage_layer_(
          new column::NumericStorage<ColumnType::submission_id::non_optional_stored_type>(
            &submission_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::submission_id::stored_type>::ToColumnType(),
            false)),
        hw_queue_id_storage_layer_(
          new column::NumericStorage<ColumnType::hw_queue_id::non_optional_stored_type>(
            &hw_queue_id_.non_null_vector(),
            ColumnTypeHelper<ColumnType::hw_queue_id::stored_type>::ToColumnType(),
            false)),
        render_subpasses_storage_layer_(
          new column::StringStorage(string_pool(), &render_subpasses_.vector()))
,
        context_id_null_layer_(new column::NullOverlay(context_id_.bv())),
        render_target_null_layer_(new column::NullOverlay(render_target_.bv())),
        render_pass_null_layer_(new column::NullOverlay(render_pass_.bv())),
        command_buffer_null_layer_(new column::NullOverlay(command_buffer_.bv())),
        frame_id_null_layer_(new column::NullOverlay(frame_id_.bv())),
        submission_id_null_layer_(new column::NullOverlay(submission_id_.bv())),
        hw_queue_id_null_layer_(new column::NullOverlay(hw_queue_id_.bv())) {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::context_id::stored_type>(
          ColumnFlag::context_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_target::stored_type>(
          ColumnFlag::render_target),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_target_name::stored_type>(
          ColumnFlag::render_target_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_pass::stored_type>(
          ColumnFlag::render_pass),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_pass_name::stored_type>(
          ColumnFlag::render_pass_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::command_buffer::stored_type>(
          ColumnFlag::command_buffer),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::command_buffer_name::stored_type>(
          ColumnFlag::command_buffer_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::frame_id::stored_type>(
          ColumnFlag::frame_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::submission_id::stored_type>(
          ColumnFlag::submission_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::hw_queue_id::stored_type>(
          ColumnFlag::hw_queue_id),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::render_subpasses::stored_type>(
          ColumnFlag::render_subpasses),
        "Column type and flag combination is not valid");
    PERFETTO_DCHECK(context_id.size() == parent_overlay.size());
    context_id_ = std::move(context_id);
    PERFETTO_DCHECK(render_target.size() == parent_overlay.size());
    render_target_ = std::move(render_target);
    PERFETTO_DCHECK(render_target_name.size() == parent_overlay.size());
    render_target_name_ = std::move(render_target_name);
    PERFETTO_DCHECK(render_pass.size() == parent_overlay.size());
    render_pass_ = std::move(render_pass);
    PERFETTO_DCHECK(render_pass_name.size() == parent_overlay.size());
    render_pass_name_ = std::move(render_pass_name);
    PERFETTO_DCHECK(command_buffer.size() == parent_overlay.size());
    command_buffer_ = std::move(command_buffer);
    PERFETTO_DCHECK(command_buffer_name.size() == parent_overlay.size());
    command_buffer_name_ = std::move(command_buffer_name);
    PERFETTO_DCHECK(frame_id.size() == parent_overlay.size());
    frame_id_ = std::move(frame_id);
    PERFETTO_DCHECK(submission_id.size() == parent_overlay.size());
    submission_id_ = std::move(submission_id);
    PERFETTO_DCHECK(hw_queue_id.size() == parent_overlay.size());
    hw_queue_id_ = std::move(hw_queue_id);
    PERFETTO_DCHECK(render_subpasses.size() == parent_overlay.size());
    render_subpasses_ = std::move(render_subpasses);

    std::vector<RefPtr<column::DataLayer>> overlay_layers(OverlayCount(&parent) + 1);
    for (uint32_t i = 0; i < overlay_layers.size(); ++i) {
      if (overlays()[i].row_map().IsIndexVector()) {
        overlay_layers[i].reset(new column::ArrangementOverlay(
            overlays()[i].row_map().GetIfIndexVector(),
            column::DataLayerChain::Indices::State::kNonmonotonic));
      } else if (overlays()[i].row_map().IsBitVector()) {
        overlay_layers[i].reset(new column::SelectorOverlay(
            overlays()[i].row_map().GetIfBitVector()));
      } else if (overlays()[i].row_map().IsRange()) {
        overlay_layers[i].reset(new column::RangeOverlay(
            overlays()[i].row_map().GetIfIRange()));
      }
    }

    OnConstructionCompleted(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],context_id_storage_layer_,render_target_storage_layer_,render_target_name_storage_layer_,render_pass_storage_layer_,render_pass_name_storage_layer_,command_buffer_storage_layer_,command_buffer_name_storage_layer_,frame_id_storage_layer_,submission_id_storage_layer_,hw_queue_id_storage_layer_,render_subpasses_storage_layer_}, {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],context_id_null_layer_,render_target_null_layer_,{},render_pass_null_layer_,{},command_buffer_null_layer_,{},frame_id_null_layer_,submission_id_null_layer_,hw_queue_id_null_layer_,{}}, std::move(overlay_layers));
  }
  SliceTable* parent_ = nullptr;
  const SliceTable* const_parent_ = nullptr;
  ColumnStorage<ColumnType::context_id::stored_type> context_id_;
  ColumnStorage<ColumnType::render_target::stored_type> render_target_;
  ColumnStorage<ColumnType::render_target_name::stored_type> render_target_name_;
  ColumnStorage<ColumnType::render_pass::stored_type> render_pass_;
  ColumnStorage<ColumnType::render_pass_name::stored_type> render_pass_name_;
  ColumnStorage<ColumnType::command_buffer::stored_type> command_buffer_;
  ColumnStorage<ColumnType::command_buffer_name::stored_type> command_buffer_name_;
  ColumnStorage<ColumnType::frame_id::stored_type> frame_id_;
  ColumnStorage<ColumnType::submission_id::stored_type> submission_id_;
  ColumnStorage<ColumnType::hw_queue_id::stored_type> hw_queue_id_;
  ColumnStorage<ColumnType::render_subpasses::stored_type> render_subpasses_;

  RefPtr<column::DataLayer> context_id_storage_layer_;
  RefPtr<column::DataLayer> render_target_storage_layer_;
  RefPtr<column::DataLayer> render_target_name_storage_layer_;
  RefPtr<column::DataLayer> render_pass_storage_layer_;
  RefPtr<column::DataLayer> render_pass_name_storage_layer_;
  RefPtr<column::DataLayer> command_buffer_storage_layer_;
  RefPtr<column::DataLayer> command_buffer_name_storage_layer_;
  RefPtr<column::DataLayer> frame_id_storage_layer_;
  RefPtr<column::DataLayer> submission_id_storage_layer_;
  RefPtr<column::DataLayer> hw_queue_id_storage_layer_;
  RefPtr<column::DataLayer> render_subpasses_storage_layer_;

  RefPtr<column::DataLayer> context_id_null_layer_;
  RefPtr<column::DataLayer> render_target_null_layer_;
  RefPtr<column::DataLayer> render_pass_null_layer_;
  RefPtr<column::DataLayer> command_buffer_null_layer_;
  RefPtr<column::DataLayer> frame_id_null_layer_;
  RefPtr<column::DataLayer> submission_id_null_layer_;
  RefPtr<column::DataLayer> hw_queue_id_null_layer_;
};
  

class GraphicsFrameSliceTable : public macros_internal::MacroTable {
 public:
  static constexpr uint32_t kColumnCount = 21;

  using Id = SliceTable::Id;
    
  struct ColumnIndex {
    static constexpr uint32_t id = 0;
    static constexpr uint32_t type = 1;
    static constexpr uint32_t ts = 2;
    static constexpr uint32_t dur = 3;
    static constexpr uint32_t track_id = 4;
    static constexpr uint32_t category = 5;
    static constexpr uint32_t name = 6;
    static constexpr uint32_t depth = 7;
    static constexpr uint32_t stack_id = 8;
    static constexpr uint32_t parent_stack_id = 9;
    static constexpr uint32_t parent_id = 10;
    static constexpr uint32_t arg_set_id = 11;
    static constexpr uint32_t thread_ts = 12;
    static constexpr uint32_t thread_dur = 13;
    static constexpr uint32_t thread_instruction_count = 14;
    static constexpr uint32_t thread_instruction_delta = 15;
    static constexpr uint32_t frame_number = 16;
    static constexpr uint32_t layer_name = 17;
    static constexpr uint32_t queue_to_acquire_time = 18;
    static constexpr uint32_t acquire_to_latch_time = 19;
    static constexpr uint32_t latch_to_present_time = 20;
  };
  struct ColumnType {
    using id = IdColumn<GraphicsFrameSliceTable::Id>;
    using type = TypedColumn<StringPool::Id>;
    using ts = TypedColumn<int64_t>;
    using dur = TypedColumn<int64_t>;
    using track_id = TypedColumn<TrackTable::Id>;
    using category = TypedColumn<std::optional<StringPool::Id>>;
    using name = TypedColumn<std::optional<StringPool::Id>>;
    using depth = TypedColumn<uint32_t>;
    using stack_id = TypedColumn<int64_t>;
    using parent_stack_id = TypedColumn<int64_t>;
    using parent_id = TypedColumn<std::optional<GraphicsFrameSliceTable::Id>>;
    using arg_set_id = TypedColumn<uint32_t>;
    using thread_ts = TypedColumn<std::optional<int64_t>>;
    using thread_dur = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_count = TypedColumn<std::optional<int64_t>>;
    using thread_instruction_delta = TypedColumn<std::optional<int64_t>>;
    using frame_number = TypedColumn<uint32_t>;
    using layer_name = TypedColumn<StringPool::Id>;
    using queue_to_acquire_time = TypedColumn<int64_t>;
    using acquire_to_latch_time = TypedColumn<int64_t>;
    using latch_to_present_time = TypedColumn<int64_t>;
  };
  struct Row : public SliceTable::Row {
    Row(int64_t in_ts = {},
        int64_t in_dur = {},
        TrackTable::Id in_track_id = {},
        std::optional<StringPool::Id> in_category = {},
        std::optional<StringPool::Id> in_name = {},
        uint32_t in_depth = {},
        int64_t in_stack_id = {},
        int64_t in_parent_stack_id = {},
        std::optional<GraphicsFrameSliceTable::Id> in_parent_id = {},
        uint32_t in_arg_set_id = {},
        std::optional<int64_t> in_thread_ts = {},
        std::optional<int64_t> in_thread_dur = {},
        std::optional<int64_t> in_thread_instruction_count = {},
        std::optional<int64_t> in_thread_instruction_delta = {},
        uint32_t in_frame_number = {},
        StringPool::Id in_layer_name = {},
        int64_t in_queue_to_acquire_time = {},
        int64_t in_acquire_to_latch_time = {},
        int64_t in_latch_to_present_time = {},
        std::nullptr_t = nullptr)
        : SliceTable::Row(in_ts, in_dur, in_track_id, in_category, in_name, in_depth, in_stack_id, in_parent_stack_id, in_parent_id, in_arg_set_id, in_thread_ts, in_thread_dur, in_thread_instruction_count, in_thread_instruction_delta),
          frame_number(in_frame_number),
          layer_name(in_layer_name),
          queue_to_acquire_time(in_queue_to_acquire_time),
          acquire_to_latch_time(in_acquire_to_latch_time),
          latch_to_present_time(in_latch_to_present_time) {
      type_ = "frame_slice";
    }
    uint32_t frame_number;
    StringPool::Id layer_name;
    int64_t queue_to_acquire_time;
    int64_t acquire_to_latch_time;
    int64_t latch_to_present_time;

    bool operator==(const GraphicsFrameSliceTable::Row& other) const {
      return type() == other.type() && ColumnType::ts::Equals(ts, other.ts) &&
       ColumnType::dur::Equals(dur, other.dur) &&
       ColumnType::track_id::Equals(track_id, other.track_id) &&
       ColumnType::category::Equals(category, other.category) &&
       ColumnType::name::Equals(name, other.name) &&
       ColumnType::depth::Equals(depth, other.depth) &&
       ColumnType::stack_id::Equals(stack_id, other.stack_id) &&
       ColumnType::parent_stack_id::Equals(parent_stack_id, other.parent_stack_id) &&
       ColumnType::parent_id::Equals(parent_id, other.parent_id) &&
       ColumnType::arg_set_id::Equals(arg_set_id, other.arg_set_id) &&
       ColumnType::thread_ts::Equals(thread_ts, other.thread_ts) &&
       ColumnType::thread_dur::Equals(thread_dur, other.thread_dur) &&
       ColumnType::thread_instruction_count::Equals(thread_instruction_count, other.thread_instruction_count) &&
       ColumnType::thread_instruction_delta::Equals(thread_instruction_delta, other.thread_instruction_delta) &&
       ColumnType::frame_number::Equals(frame_number, other.frame_number) &&
       ColumnType::layer_name::Equals(layer_name, other.layer_name) &&
       ColumnType::queue_to_acquire_time::Equals(queue_to_acquire_time, other.queue_to_acquire_time) &&
       ColumnType::acquire_to_latch_time::Equals(acquire_to_latch_time, other.acquire_to_latch_time) &&
       ColumnType::latch_to_present_time::Equals(latch_to_present_time, other.latch_to_present_time);
    }
  };
  struct ColumnFlag {
    static constexpr uint32_t frame_number = ColumnType::frame_number::default_flags();
    static constexpr uint32_t layer_name = ColumnType::layer_name::default_flags();
    static constexpr uint32_t queue_to_acquire_time = ColumnType::queue_to_acquire_time::default_flags();
    static constexpr uint32_t acquire_to_latch_time = ColumnType::acquire_to_latch_time::default_flags();
    static constexpr uint32_t latch_to_present_time = ColumnType::latch_to_present_time::default_flags();
  };

  class RowNumber;
  class ConstRowReference;
  class RowReference;

  class RowNumber : public macros_internal::AbstractRowNumber<
      GraphicsFrameSliceTable, ConstRowReference, RowReference> {
   public:
    explicit RowNumber(uint32_t row_number)
        : AbstractRowNumber(row_number) {}
  };
  static_assert(std::is_trivially_destructible_v<RowNumber>,
                "Inheritance used without trivial destruction");

  class ConstRowReference : public macros_internal::AbstractConstRowReference<
    GraphicsFrameSliceTable, RowNumber> {
   public:
    ConstRowReference(const GraphicsFrameSliceTable* table, uint32_t row_number)
        : AbstractConstRowReference(table, row_number) {}

    ColumnType::id::type id() const {
      return table_->id()[row_number_];
    }
    ColumnType::type::type type() const {
      return table_->type()[row_number_];
    }
    ColumnType::ts::type ts() const {
      return table_->ts()[row_number_];
    }
    ColumnType::dur::type dur() const {
      return table_->dur()[row_number_];
    }
    ColumnType::track_id::type track_id() const {
      return table_->track_id()[row_number_];
    }
    ColumnType::category::type category() const {
      return table_->category()[row_number_];
    }
    ColumnType::name::type name() const {
      return table_->name()[row_number_];
    }
    ColumnType::depth::type depth() const {
      return table_->depth()[row_number_];
    }
    ColumnType::stack_id::type stack_id() const {
      return table_->stack_id()[row_number_];
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      return table_->parent_stack_id()[row_number_];
    }
    ColumnType::parent_id::type parent_id() const {
      return table_->parent_id()[row_number_];
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      return table_->arg_set_id()[row_number_];
    }
    ColumnType::thread_ts::type thread_ts() const {
      return table_->thread_ts()[row_number_];
    }
    ColumnType::thread_dur::type thread_dur() const {
      return table_->thread_dur()[row_number_];
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      return table_->thread_instruction_count()[row_number_];
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      return table_->thread_instruction_delta()[row_number_];
    }
    ColumnType::frame_number::type frame_number() const {
      return table_->frame_number()[row_number_];
    }
    ColumnType::layer_name::type layer_name() const {
      return table_->layer_name()[row_number_];
    }
    ColumnType::queue_to_acquire_time::type queue_to_acquire_time() const {
      return table_->queue_to_acquire_time()[row_number_];
    }
    ColumnType::acquire_to_latch_time::type acquire_to_latch_time() const {
      return table_->acquire_to_latch_time()[row_number_];
    }
    ColumnType::latch_to_present_time::type latch_to_present_time() const {
      return table_->latch_to_present_time()[row_number_];
    }
  };
  static_assert(std::is_trivially_destructible_v<ConstRowReference>,
                "Inheritance used without trivial destruction");
  class RowReference : public ConstRowReference {
   public:
    RowReference(const GraphicsFrameSliceTable* table, uint32_t row_number)
        : ConstRowReference(table, row_number) {}

    void set_ts(
        ColumnType::ts::non_optional_type v) {
      return mutable_table()->mutable_ts()->Set(row_number_, v);
    }
    void set_dur(
        ColumnType::dur::non_optional_type v) {
      return mutable_table()->mutable_dur()->Set(row_number_, v);
    }
    void set_track_id(
        ColumnType::track_id::non_optional_type v) {
      return mutable_table()->mutable_track_id()->Set(row_number_, v);
    }
    void set_category(
        ColumnType::category::non_optional_type v) {
      return mutable_table()->mutable_category()->Set(row_number_, v);
    }
    void set_name(
        ColumnType::name::non_optional_type v) {
      return mutable_table()->mutable_name()->Set(row_number_, v);
    }
    void set_depth(
        ColumnType::depth::non_optional_type v) {
      return mutable_table()->mutable_depth()->Set(row_number_, v);
    }
    void set_stack_id(
        ColumnType::stack_id::non_optional_type v) {
      return mutable_table()->mutable_stack_id()->Set(row_number_, v);
    }
    void set_parent_stack_id(
        ColumnType::parent_stack_id::non_optional_type v) {
      return mutable_table()->mutable_parent_stack_id()->Set(row_number_, v);
    }
    void set_parent_id(
        ColumnType::parent_id::non_optional_type v) {
      return mutable_table()->mutable_parent_id()->Set(row_number_, v);
    }
    void set_arg_set_id(
        ColumnType::arg_set_id::non_optional_type v) {
      return mutable_table()->mutable_arg_set_id()->Set(row_number_, v);
    }
    void set_thread_ts(
        ColumnType::thread_ts::non_optional_type v) {
      return mutable_table()->mutable_thread_ts()->Set(row_number_, v);
    }
    void set_thread_dur(
        ColumnType::thread_dur::non_optional_type v) {
      return mutable_table()->mutable_thread_dur()->Set(row_number_, v);
    }
    void set_thread_instruction_count(
        ColumnType::thread_instruction_count::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_count()->Set(row_number_, v);
    }
    void set_thread_instruction_delta(
        ColumnType::thread_instruction_delta::non_optional_type v) {
      return mutable_table()->mutable_thread_instruction_delta()->Set(row_number_, v);
    }
    void set_frame_number(
        ColumnType::frame_number::non_optional_type v) {
      return mutable_table()->mutable_frame_number()->Set(row_number_, v);
    }
    void set_layer_name(
        ColumnType::layer_name::non_optional_type v) {
      return mutable_table()->mutable_layer_name()->Set(row_number_, v);
    }
    void set_queue_to_acquire_time(
        ColumnType::queue_to_acquire_time::non_optional_type v) {
      return mutable_table()->mutable_queue_to_acquire_time()->Set(row_number_, v);
    }
    void set_acquire_to_latch_time(
        ColumnType::acquire_to_latch_time::non_optional_type v) {
      return mutable_table()->mutable_acquire_to_latch_time()->Set(row_number_, v);
    }
    void set_latch_to_present_time(
        ColumnType::latch_to_present_time::non_optional_type v) {
      return mutable_table()->mutable_latch_to_present_time()->Set(row_number_, v);
    }

   private:
    GraphicsFrameSliceTable* mutable_table() const {
      return const_cast<GraphicsFrameSliceTable*>(table_);
    }
  };
  static_assert(std::is_trivially_destructible_v<RowReference>,
                "Inheritance used without trivial destruction");

  class ConstIterator;
  class ConstIterator : public macros_internal::AbstractConstIterator<
    ConstIterator, GraphicsFrameSliceTable, RowNumber, ConstRowReference> {
   public:
    ColumnType::id::type id() const {
      const auto& col = table_->id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::type::type type() const {
      const auto& col = table_->type();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::ts::type ts() const {
      const auto& col = table_->ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::dur::type dur() const {
      const auto& col = table_->dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::track_id::type track_id() const {
      const auto& col = table_->track_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::category::type category() const {
      const auto& col = table_->category();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::name::type name() const {
      const auto& col = table_->name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::depth::type depth() const {
      const auto& col = table_->depth();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::stack_id::type stack_id() const {
      const auto& col = table_->stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_stack_id::type parent_stack_id() const {
      const auto& col = table_->parent_stack_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::parent_id::type parent_id() const {
      const auto& col = table_->parent_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::arg_set_id::type arg_set_id() const {
      const auto& col = table_->arg_set_id();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_ts::type thread_ts() const {
      const auto& col = table_->thread_ts();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_dur::type thread_dur() const {
      const auto& col = table_->thread_dur();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_count::type thread_instruction_count() const {
      const auto& col = table_->thread_instruction_count();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::thread_instruction_delta::type thread_instruction_delta() const {
      const auto& col = table_->thread_instruction_delta();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::frame_number::type frame_number() const {
      const auto& col = table_->frame_number();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::layer_name::type layer_name() const {
      const auto& col = table_->layer_name();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::queue_to_acquire_time::type queue_to_acquire_time() const {
      const auto& col = table_->queue_to_acquire_time();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::acquire_to_latch_time::type acquire_to_latch_time() const {
      const auto& col = table_->acquire_to_latch_time();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }
    ColumnType::latch_to_present_time::type latch_to_present_time() const {
      const auto& col = table_->latch_to_present_time();
      return col.GetAtIdx(
        iterator_.StorageIndexForColumn(col.index_in_table()));
    }

   protected:
    explicit ConstIterator(const GraphicsFrameSliceTable* table,
                           Table::Iterator iterator)
        : AbstractConstIterator(table, std::move(iterator)) {}

    uint32_t CurrentRowNumber() const {
      return iterator_.StorageIndexForLastOverlay();
    }

   private:
    friend class GraphicsFrameSliceTable;
    friend class macros_internal::AbstractConstIterator<
      ConstIterator, GraphicsFrameSliceTable, RowNumber, ConstRowReference>;
  };
  class Iterator : public ConstIterator {
    public:
     void set_ts(ColumnType::ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_dur(ColumnType::dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_track_id(ColumnType::track_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_track_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_category(ColumnType::category::non_optional_type v) {
        auto* col = mutable_table_->mutable_category();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_name(ColumnType::name::non_optional_type v) {
        auto* col = mutable_table_->mutable_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_depth(ColumnType::depth::non_optional_type v) {
        auto* col = mutable_table_->mutable_depth();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_stack_id(ColumnType::stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_stack_id(ColumnType::parent_stack_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_stack_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_parent_id(ColumnType::parent_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_parent_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_arg_set_id(ColumnType::arg_set_id::non_optional_type v) {
        auto* col = mutable_table_->mutable_arg_set_id();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_ts(ColumnType::thread_ts::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_ts();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_dur(ColumnType::thread_dur::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_dur();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_count(ColumnType::thread_instruction_count::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_count();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_thread_instruction_delta(ColumnType::thread_instruction_delta::non_optional_type v) {
        auto* col = mutable_table_->mutable_thread_instruction_delta();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_frame_number(ColumnType::frame_number::non_optional_type v) {
        auto* col = mutable_table_->mutable_frame_number();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_layer_name(ColumnType::layer_name::non_optional_type v) {
        auto* col = mutable_table_->mutable_layer_name();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_queue_to_acquire_time(ColumnType::queue_to_acquire_time::non_optional_type v) {
        auto* col = mutable_table_->mutable_queue_to_acquire_time();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_acquire_to_latch_time(ColumnType::acquire_to_latch_time::non_optional_type v) {
        auto* col = mutable_table_->mutable_acquire_to_latch_time();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }
      void set_latch_to_present_time(ColumnType::latch_to_present_time::non_optional_type v) {
        auto* col = mutable_table_->mutable_latch_to_present_time();
        col->SetAtIdx(
          iterator_.StorageIndexForColumn(col->index_in_table()), v);
      }

     RowReference row_reference() const {
       return RowReference(mutable_table_, CurrentRowNumber());
     }

    private:
     friend class GraphicsFrameSliceTable;

     explicit Iterator(GraphicsFrameSliceTable* table, Table::Iterator iterator)
        : ConstIterator(table, std::move(iterator)),
          mutable_table_(table) {}

     GraphicsFrameSliceTable* mutable_table_ = nullptr;
  };

  struct IdAndRow {
    Id id;
    uint32_t row;
    RowReference row_reference;
    RowNumber row_number;
  };

  static std::vector<ColumnLegacy> GetColumns(
      GraphicsFrameSliceTable* self,
      const macros_internal::MacroTable* parent) {
    std::vector<ColumnLegacy> columns =
        CopyColumnsFromParentOrAddRootColumns(self, parent);
    uint32_t olay_idx = OverlayCount(parent);
    AddColumnToVector(columns, "frame_number", &self->frame_number_, ColumnFlag::frame_number,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "layer_name", &self->layer_name_, ColumnFlag::layer_name,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "queue_to_acquire_time", &self->queue_to_acquire_time_, ColumnFlag::queue_to_acquire_time,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "acquire_to_latch_time", &self->acquire_to_latch_time_, ColumnFlag::acquire_to_latch_time,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    AddColumnToVector(columns, "latch_to_present_time", &self->latch_to_present_time_, ColumnFlag::latch_to_present_time,
                      static_cast<uint32_t>(columns.size()), olay_idx);
    return columns;
  }

  PERFETTO_NO_INLINE explicit GraphicsFrameSliceTable(StringPool* pool, SliceTable* parent)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, parent),
          parent),
        parent_(parent), const_parent_(parent), frame_number_(ColumnStorage<ColumnType::frame_number::stored_type>::Create<false>()),
        layer_name_(ColumnStorage<ColumnType::layer_name::stored_type>::Create<false>()),
        queue_to_acquire_time_(ColumnStorage<ColumnType::queue_to_acquire_time::stored_type>::Create<false>()),
        acquire_to_latch_time_(ColumnStorage<ColumnType::acquire_to_latch_time::stored_type>::Create<false>()),
        latch_to_present_time_(ColumnStorage<ColumnType::latch_to_present_time::stored_type>::Create<false>())
,
        frame_number_storage_layer_(
        new column::NumericStorage<ColumnType::frame_number::non_optional_stored_type>(
          &frame_number_.vector(),
          ColumnTypeHelper<ColumnType::frame_number::stored_type>::ToColumnType(),
          false)),
        layer_name_storage_layer_(
          new column::StringStorage(string_pool(), &layer_name_.vector())),
        queue_to_acquire_time_storage_layer_(
        new column::NumericStorage<ColumnType::queue_to_acquire_time::non_optional_stored_type>(
          &queue_to_acquire_time_.vector(),
          ColumnTypeHelper<ColumnType::queue_to_acquire_time::stored_type>::ToColumnType(),
          false)),
        acquire_to_latch_time_storage_layer_(
        new column::NumericStorage<ColumnType::acquire_to_latch_time::non_optional_stored_type>(
          &acquire_to_latch_time_.vector(),
          ColumnTypeHelper<ColumnType::acquire_to_latch_time::stored_type>::ToColumnType(),
          false)),
        latch_to_present_time_storage_layer_(
        new column::NumericStorage<ColumnType::latch_to_present_time::non_optional_stored_type>(
          &latch_to_present_time_.vector(),
          ColumnTypeHelper<ColumnType::latch_to_present_time::stored_type>::ToColumnType(),
          false))
         {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::frame_number::stored_type>(
          ColumnFlag::frame_number),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::layer_name::stored_type>(
          ColumnFlag::layer_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::queue_to_acquire_time::stored_type>(
          ColumnFlag::queue_to_acquire_time),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::acquire_to_latch_time::stored_type>(
          ColumnFlag::acquire_to_latch_time),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::latch_to_present_time::stored_type>(
          ColumnFlag::latch_to_present_time),
        "Column type and flag combination is not valid");
    OnConstructionCompletedRegularConstructor(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],frame_number_storage_layer_,layer_name_storage_layer_,queue_to_acquire_time_storage_layer_,acquire_to_latch_time_storage_layer_,latch_to_present_time_storage_layer_},
      {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],{},{},{},{},{}});
  }
  ~GraphicsFrameSliceTable() override;

  static const char* Name() { return "frame_slice"; }

  static Table::Schema ComputeStaticSchema() {
    Table::Schema schema;
    schema.columns.emplace_back(Table::Schema::Column{
        "id", SqlValue::Type::kLong, true, true, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "type", SqlValue::Type::kString, false, false, false, false});
    schema.columns.emplace_back(Table::Schema::Column{
        "ts", ColumnType::ts::SqlValueType(), false,
        true,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "dur", ColumnType::dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "track_id", ColumnType::track_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "category", ColumnType::category::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "name", ColumnType::name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "depth", ColumnType::depth::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "stack_id", ColumnType::stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_stack_id", ColumnType::parent_stack_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "parent_id", ColumnType::parent_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "arg_set_id", ColumnType::arg_set_id::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_ts", ColumnType::thread_ts::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_dur", ColumnType::thread_dur::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_count", ColumnType::thread_instruction_count::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "thread_instruction_delta", ColumnType::thread_instruction_delta::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "frame_number", ColumnType::frame_number::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "layer_name", ColumnType::layer_name::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "queue_to_acquire_time", ColumnType::queue_to_acquire_time::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "acquire_to_latch_time", ColumnType::acquire_to_latch_time::SqlValueType(), false,
        false,
        false,
        false});
    schema.columns.emplace_back(Table::Schema::Column{
        "latch_to_present_time", ColumnType::latch_to_present_time::SqlValueType(), false,
        false,
        false,
        false});
    return schema;
  }

  ConstIterator IterateRows() const {
    return ConstIterator(this, Table::IterateRows());
  }

  Iterator IterateRows() { return Iterator(this, Table::IterateRows()); }

  ConstIterator FilterToIterator(
      const std::vector<Constraint>& cs) const {
    return ConstIterator(
      this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  Iterator FilterToIterator(
      const std::vector<Constraint>& cs) {
    return Iterator(this, ApplyAndIterateRows(QueryToRowMap(cs, {})));
  }

  void ShrinkToFit() {
    frame_number_.ShrinkToFit();
    layer_name_.ShrinkToFit();
    queue_to_acquire_time_.ShrinkToFit();
    acquire_to_latch_time_.ShrinkToFit();
    latch_to_present_time_.ShrinkToFit();
  }

  std::optional<ConstRowReference> FindById(Id find_id) const {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(ConstRowReference(this, *row))
               : std::nullopt;
  }

  std::optional<RowReference> FindById(Id find_id) {
    std::optional<uint32_t> row = id().IndexOf(find_id);
    return row ? std::make_optional(RowReference(this, *row)) : std::nullopt;
  }

  IdAndRow Insert(const Row& row) {
    uint32_t row_number = row_count();
    Id id = Id{parent_->Insert(row).id};
    UpdateOverlaysAfterParentInsert();
    mutable_frame_number()->Append(row.frame_number);
    mutable_layer_name()->Append(row.layer_name);
    mutable_queue_to_acquire_time()->Append(row.queue_to_acquire_time);
    mutable_acquire_to_latch_time()->Append(row.acquire_to_latch_time);
    mutable_latch_to_present_time()->Append(row.latch_to_present_time);
    UpdateSelfOverlayAfterInsert();
    return IdAndRow{id, row_number, RowReference(this, row_number),
                     RowNumber(row_number)};
  }

  static std::unique_ptr<Table> ExtendParent(
      const SliceTable& parent,
      ColumnStorage<ColumnType::frame_number::stored_type> frame_number
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name
, ColumnStorage<ColumnType::queue_to_acquire_time::stored_type> queue_to_acquire_time
, ColumnStorage<ColumnType::acquire_to_latch_time::stored_type> acquire_to_latch_time
, ColumnStorage<ColumnType::latch_to_present_time::stored_type> latch_to_present_time) {
    return std::unique_ptr<Table>(new GraphicsFrameSliceTable(
        parent.string_pool(), parent, RowMap(0, parent.row_count()),
        std::move(frame_number), std::move(layer_name), std::move(queue_to_acquire_time), std::move(acquire_to_latch_time), std::move(latch_to_present_time)));
  }

  static std::unique_ptr<Table> SelectAndExtendParent(
      const SliceTable& parent,
      std::vector<SliceTable::RowNumber> parent_overlay,
      ColumnStorage<ColumnType::frame_number::stored_type> frame_number
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name
, ColumnStorage<ColumnType::queue_to_acquire_time::stored_type> queue_to_acquire_time
, ColumnStorage<ColumnType::acquire_to_latch_time::stored_type> acquire_to_latch_time
, ColumnStorage<ColumnType::latch_to_present_time::stored_type> latch_to_present_time) {
    std::vector<uint32_t> prs_untyped(parent_overlay.size());
    for (uint32_t i = 0; i < parent_overlay.size(); ++i) {
      prs_untyped[i] = parent_overlay[i].row_number();
    }
    return std::unique_ptr<Table>(new GraphicsFrameSliceTable(
        parent.string_pool(), parent, RowMap(std::move(prs_untyped)),
        std::move(frame_number), std::move(layer_name), std::move(queue_to_acquire_time), std::move(acquire_to_latch_time), std::move(latch_to_present_time)));
  }

  const IdColumn<GraphicsFrameSliceTable::Id>& id() const {
    return static_cast<const ColumnType::id&>(columns()[ColumnIndex::id]);
  }
  const TypedColumn<StringPool::Id>& type() const {
    return static_cast<const ColumnType::type&>(columns()[ColumnIndex::type]);
  }
  const TypedColumn<int64_t>& ts() const {
    return static_cast<const ColumnType::ts&>(columns()[ColumnIndex::ts]);
  }
  const TypedColumn<int64_t>& dur() const {
    return static_cast<const ColumnType::dur&>(columns()[ColumnIndex::dur]);
  }
  const TypedColumn<TrackTable::Id>& track_id() const {
    return static_cast<const ColumnType::track_id&>(columns()[ColumnIndex::track_id]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& category() const {
    return static_cast<const ColumnType::category&>(columns()[ColumnIndex::category]);
  }
  const TypedColumn<std::optional<StringPool::Id>>& name() const {
    return static_cast<const ColumnType::name&>(columns()[ColumnIndex::name]);
  }
  const TypedColumn<uint32_t>& depth() const {
    return static_cast<const ColumnType::depth&>(columns()[ColumnIndex::depth]);
  }
  const TypedColumn<int64_t>& stack_id() const {
    return static_cast<const ColumnType::stack_id&>(columns()[ColumnIndex::stack_id]);
  }
  const TypedColumn<int64_t>& parent_stack_id() const {
    return static_cast<const ColumnType::parent_stack_id&>(columns()[ColumnIndex::parent_stack_id]);
  }
  const TypedColumn<std::optional<GraphicsFrameSliceTable::Id>>& parent_id() const {
    return static_cast<const ColumnType::parent_id&>(columns()[ColumnIndex::parent_id]);
  }
  const TypedColumn<uint32_t>& arg_set_id() const {
    return static_cast<const ColumnType::arg_set_id&>(columns()[ColumnIndex::arg_set_id]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_ts() const {
    return static_cast<const ColumnType::thread_ts&>(columns()[ColumnIndex::thread_ts]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_dur() const {
    return static_cast<const ColumnType::thread_dur&>(columns()[ColumnIndex::thread_dur]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_count() const {
    return static_cast<const ColumnType::thread_instruction_count&>(columns()[ColumnIndex::thread_instruction_count]);
  }
  const TypedColumn<std::optional<int64_t>>& thread_instruction_delta() const {
    return static_cast<const ColumnType::thread_instruction_delta&>(columns()[ColumnIndex::thread_instruction_delta]);
  }
  const TypedColumn<uint32_t>& frame_number() const {
    return static_cast<const ColumnType::frame_number&>(columns()[ColumnIndex::frame_number]);
  }
  const TypedColumn<StringPool::Id>& layer_name() const {
    return static_cast<const ColumnType::layer_name&>(columns()[ColumnIndex::layer_name]);
  }
  const TypedColumn<int64_t>& queue_to_acquire_time() const {
    return static_cast<const ColumnType::queue_to_acquire_time&>(columns()[ColumnIndex::queue_to_acquire_time]);
  }
  const TypedColumn<int64_t>& acquire_to_latch_time() const {
    return static_cast<const ColumnType::acquire_to_latch_time&>(columns()[ColumnIndex::acquire_to_latch_time]);
  }
  const TypedColumn<int64_t>& latch_to_present_time() const {
    return static_cast<const ColumnType::latch_to_present_time&>(columns()[ColumnIndex::latch_to_present_time]);
  }

  TypedColumn<int64_t>* mutable_ts() {
    return static_cast<ColumnType::ts*>(
        GetColumn(ColumnIndex::ts));
  }
  TypedColumn<int64_t>* mutable_dur() {
    return static_cast<ColumnType::dur*>(
        GetColumn(ColumnIndex::dur));
  }
  TypedColumn<TrackTable::Id>* mutable_track_id() {
    return static_cast<ColumnType::track_id*>(
        GetColumn(ColumnIndex::track_id));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_category() {
    return static_cast<ColumnType::category*>(
        GetColumn(ColumnIndex::category));
  }
  TypedColumn<std::optional<StringPool::Id>>* mutable_name() {
    return static_cast<ColumnType::name*>(
        GetColumn(ColumnIndex::name));
  }
  TypedColumn<uint32_t>* mutable_depth() {
    return static_cast<ColumnType::depth*>(
        GetColumn(ColumnIndex::depth));
  }
  TypedColumn<int64_t>* mutable_stack_id() {
    return static_cast<ColumnType::stack_id*>(
        GetColumn(ColumnIndex::stack_id));
  }
  TypedColumn<int64_t>* mutable_parent_stack_id() {
    return static_cast<ColumnType::parent_stack_id*>(
        GetColumn(ColumnIndex::parent_stack_id));
  }
  TypedColumn<std::optional<GraphicsFrameSliceTable::Id>>* mutable_parent_id() {
    return static_cast<ColumnType::parent_id*>(
        GetColumn(ColumnIndex::parent_id));
  }
  TypedColumn<uint32_t>* mutable_arg_set_id() {
    return static_cast<ColumnType::arg_set_id*>(
        GetColumn(ColumnIndex::arg_set_id));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_ts() {
    return static_cast<ColumnType::thread_ts*>(
        GetColumn(ColumnIndex::thread_ts));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_dur() {
    return static_cast<ColumnType::thread_dur*>(
        GetColumn(ColumnIndex::thread_dur));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_count() {
    return static_cast<ColumnType::thread_instruction_count*>(
        GetColumn(ColumnIndex::thread_instruction_count));
  }
  TypedColumn<std::optional<int64_t>>* mutable_thread_instruction_delta() {
    return static_cast<ColumnType::thread_instruction_delta*>(
        GetColumn(ColumnIndex::thread_instruction_delta));
  }
  TypedColumn<uint32_t>* mutable_frame_number() {
    return static_cast<ColumnType::frame_number*>(
        GetColumn(ColumnIndex::frame_number));
  }
  TypedColumn<StringPool::Id>* mutable_layer_name() {
    return static_cast<ColumnType::layer_name*>(
        GetColumn(ColumnIndex::layer_name));
  }
  TypedColumn<int64_t>* mutable_queue_to_acquire_time() {
    return static_cast<ColumnType::queue_to_acquire_time*>(
        GetColumn(ColumnIndex::queue_to_acquire_time));
  }
  TypedColumn<int64_t>* mutable_acquire_to_latch_time() {
    return static_cast<ColumnType::acquire_to_latch_time*>(
        GetColumn(ColumnIndex::acquire_to_latch_time));
  }
  TypedColumn<int64_t>* mutable_latch_to_present_time() {
    return static_cast<ColumnType::latch_to_present_time*>(
        GetColumn(ColumnIndex::latch_to_present_time));
  }

 private:
  GraphicsFrameSliceTable(StringPool* pool,
            const SliceTable& parent,
            const RowMap& parent_overlay,
            ColumnStorage<ColumnType::frame_number::stored_type> frame_number
, ColumnStorage<ColumnType::layer_name::stored_type> layer_name
, ColumnStorage<ColumnType::queue_to_acquire_time::stored_type> queue_to_acquire_time
, ColumnStorage<ColumnType::acquire_to_latch_time::stored_type> acquire_to_latch_time
, ColumnStorage<ColumnType::latch_to_present_time::stored_type> latch_to_present_time)
      : macros_internal::MacroTable(
          pool,
          GetColumns(this, &parent),
          parent,
          parent_overlay),
          const_parent_(&parent)
,
        frame_number_storage_layer_(
        new column::NumericStorage<ColumnType::frame_number::non_optional_stored_type>(
          &frame_number_.vector(),
          ColumnTypeHelper<ColumnType::frame_number::stored_type>::ToColumnType(),
          false)),
        layer_name_storage_layer_(
          new column::StringStorage(string_pool(), &layer_name_.vector())),
        queue_to_acquire_time_storage_layer_(
        new column::NumericStorage<ColumnType::queue_to_acquire_time::non_optional_stored_type>(
          &queue_to_acquire_time_.vector(),
          ColumnTypeHelper<ColumnType::queue_to_acquire_time::stored_type>::ToColumnType(),
          false)),
        acquire_to_latch_time_storage_layer_(
        new column::NumericStorage<ColumnType::acquire_to_latch_time::non_optional_stored_type>(
          &acquire_to_latch_time_.vector(),
          ColumnTypeHelper<ColumnType::acquire_to_latch_time::stored_type>::ToColumnType(),
          false)),
        latch_to_present_time_storage_layer_(
        new column::NumericStorage<ColumnType::latch_to_present_time::non_optional_stored_type>(
          &latch_to_present_time_.vector(),
          ColumnTypeHelper<ColumnType::latch_to_present_time::stored_type>::ToColumnType(),
          false))
         {
    static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::frame_number::stored_type>(
          ColumnFlag::frame_number),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::layer_name::stored_type>(
          ColumnFlag::layer_name),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::queue_to_acquire_time::stored_type>(
          ColumnFlag::queue_to_acquire_time),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::acquire_to_latch_time::stored_type>(
          ColumnFlag::acquire_to_latch_time),
        "Column type and flag combination is not valid");
      static_assert(
        ColumnLegacy::IsFlagsAndTypeValid<ColumnType::latch_to_present_time::stored_type>(
          ColumnFlag::latch_to_present_time),
        "Column type and flag combination is not valid");
    PERFETTO_DCHECK(frame_number.size() == parent_overlay.size());
    frame_number_ = std::move(frame_number);
    PERFETTO_DCHECK(layer_name.size() == parent_overlay.size());
    layer_name_ = std::move(layer_name);
    PERFETTO_DCHECK(queue_to_acquire_time.size() == parent_overlay.size());
    queue_to_acquire_time_ = std::move(queue_to_acquire_time);
    PERFETTO_DCHECK(acquire_to_latch_time.size() == parent_overlay.size());
    acquire_to_latch_time_ = std::move(acquire_to_latch_time);
    PERFETTO_DCHECK(latch_to_present_time.size() == parent_overlay.size());
    latch_to_present_time_ = std::move(latch_to_present_time);

    std::vector<RefPtr<column::DataLayer>> overlay_layers(OverlayCount(&parent) + 1);
    for (uint32_t i = 0; i < overlay_layers.size(); ++i) {
      if (overlays()[i].row_map().IsIndexVector()) {
        overlay_layers[i].reset(new column::ArrangementOverlay(
            overlays()[i].row_map().GetIfIndexVector(),
            column::DataLayerChain::Indices::State::kNonmonotonic));
      } else if (overlays()[i].row_map().IsBitVector()) {
        overlay_layers[i].reset(new column::SelectorOverlay(
            overlays()[i].row_map().GetIfBitVector()));
      } else if (overlays()[i].row_map().IsRange()) {
        overlay_layers[i].reset(new column::RangeOverlay(
            overlays()[i].row_map().GetIfIRange()));
      }
    }

    OnConstructionCompleted(
      {const_parent_->storage_layers()[ColumnIndex::id],const_parent_->storage_layers()[ColumnIndex::type],const_parent_->storage_layers()[ColumnIndex::ts],const_parent_->storage_layers()[ColumnIndex::dur],const_parent_->storage_layers()[ColumnIndex::track_id],const_parent_->storage_layers()[ColumnIndex::category],const_parent_->storage_layers()[ColumnIndex::name],const_parent_->storage_layers()[ColumnIndex::depth],const_parent_->storage_layers()[ColumnIndex::stack_id],const_parent_->storage_layers()[ColumnIndex::parent_stack_id],const_parent_->storage_layers()[ColumnIndex::parent_id],const_parent_->storage_layers()[ColumnIndex::arg_set_id],const_parent_->storage_layers()[ColumnIndex::thread_ts],const_parent_->storage_layers()[ColumnIndex::thread_dur],const_parent_->storage_layers()[ColumnIndex::thread_instruction_count],const_parent_->storage_layers()[ColumnIndex::thread_instruction_delta],frame_number_storage_layer_,layer_name_storage_layer_,queue_to_acquire_time_storage_layer_,acquire_to_latch_time_storage_layer_,latch_to_present_time_storage_layer_}, {{},{},{},{},{},{},{},{},{},{},const_parent_->null_layers()[ColumnIndex::parent_id],{},const_parent_->null_layers()[ColumnIndex::thread_ts],const_parent_->null_layers()[ColumnIndex::thread_dur],const_parent_->null_layers()[ColumnIndex::thread_instruction_count],const_parent_->null_layers()[ColumnIndex::thread_instruction_delta],{},{},{},{},{}}, std::move(overlay_layers));
  }
  SliceTable* parent_ = nullptr;
  const SliceTable* const_parent_ = nullptr;
  ColumnStorage<ColumnType::frame_number::stored_type> frame_number_;
  ColumnStorage<ColumnType::layer_name::stored_type> layer_name_;
  ColumnStorage<ColumnType::queue_to_acquire_time::stored_type> queue_to_acquire_time_;
  ColumnStorage<ColumnType::acquire_to_latch_time::stored_type> acquire_to_latch_time_;
  ColumnStorage<ColumnType::latch_to_present_time::stored_type> latch_to_present_time_;

  RefPtr<column::DataLayer> frame_number_storage_layer_;
  RefPtr<column::DataLayer> layer_name_storage_layer_;
  RefPtr<column::DataLayer> queue_to_acquire_time_storage_layer_;
  RefPtr<column::DataLayer> acquire_to_latch_time_storage_layer_;
  RefPtr<column::DataLayer> latch_to_present_time_storage_layer_;

  
};

}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_TABLES_SLICE_TABLES_PY_H_
