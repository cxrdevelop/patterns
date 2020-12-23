#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <doctest/doctest.h>

class Document
{
    std::vector<std::string> data;
public:
    void insert(std::size_t line, std::string str ) {
        if (line > data.size()) throw std::runtime_error(fmt::format("Line number error. Line: {}, total: {}", line, data.size()));
        data.insert(data.begin() + line, std::move(str));
    }

    void remove(std::size_t line) {
        if (line >= data.size()) throw std::runtime_error(fmt::format("Line number error. Line: {}, total: {}", line, data.size()));
        data.erase(data.begin() + line);
    }

    std::string& operator[] (std::size_t line) noexcept {
        return data[line];
    }

    std::string& at(std::size_t line) noexcept(noexcept(data.at(line))) {
        return data.at(line);
    }

    void print() {
        fmt::print("Document: {}", data);
    }
    std::size_t size() const noexcept(noexcept(data.size())) {
        return data.size();
    }

    const auto& lines() const noexcept {
        return data;
    }
};

class Command {
public:
    virtual ~Command() = default;
    virtual void redo() = 0;
    virtual void undo() = 0;
};

class DocumentCommands : public Command {
public:
    Document *doc_;
    std::size_t line_;
    std::string str_;

    DocumentCommands(Document *doc, std::size_t line, std::string  str) : doc_{doc}, line_(line), str_(std::move(str)) {}
};


class InsertCommand : public DocumentCommands {
public:
    InsertCommand(Document *doc, std::size_t line, std::string str) : DocumentCommands{doc, line, std::move(str)} {}

    void redo() final {
        doc_->insert(line_, str_);
    }

    void undo() final {
        doc_->remove(line_);
    }
};

class RemoveCommand : public DocumentCommands {
public:
    RemoveCommand(Document *doc, std::size_t line) : DocumentCommands{doc, line, doc->at(line)} {}

    void redo() final {
        doc_->remove(line_);
    }

    void undo() final {
        doc_->insert(line_, str_);
    }
};

class Invoker {
    std::vector<std::unique_ptr<Command>> UndoCommands;
    std::vector<std::unique_ptr<Command>> RedoCommands;
    Document doc;
    bool changed_{false};

public:
    void insert(int line, std::string str) {
        auto cmd = std::make_unique<InsertCommand>(&doc, line, str);
        cmd->redo();
        UndoCommands.push_back(std::move(cmd));
        changed_ = true;
        resetRedo();
    }
    void remove(int line) {
        auto cmd = std::make_unique<RemoveCommand>(&doc, line);
        cmd->redo();
        UndoCommands.push_back(std::move(cmd));
        changed_ = true;
        resetRedo();
    }

    [[nodiscard]] bool changed() const {
        return changed_;
    }

    void reset() {
        changed_ = false;
    }

    void undo() {
        if (UndoCommands.size() == 0) {
            changed_ = false;
            return;
        }

        UndoCommands.back()->undo();
        RedoCommands.push_back(std::move(UndoCommands.back()));
        UndoCommands.pop_back();
    }

    void redo() {
        if (RedoCommands.size() == 0) return;

        RedoCommands.back()->redo();
        UndoCommands.push_back(std::move(RedoCommands.back()));
        RedoCommands.pop_back();
    }

    void print() { doc.print(); }
    [[nodiscard]] std::size_t size() const {
        return doc.size();
    }
    [[nodiscard]] const auto &lines() const noexcept {
        return doc.lines();
    }

private:
    void resetRedo() {
        RedoCommands.clear();
    }
};

TEST_CASE("test") {
    Invoker inv;

    // INSERT
    CHECK(inv.changed() == false);
    inv.insert(0, "0");
    CHECK(inv.changed() == true);
    inv.reset();
    CHECK(inv.changed() == false);
    inv.insert(1, "1");
    inv.insert(2, "2");
    CHECK(inv.changed() == true);
    CHECK(inv.lines() == std::vector<std::string>{"0","1","2"});

    // UNDO ALL
    inv.undo();
    CHECK(inv.changed() == true);
    CHECK(inv.lines() == std::vector<std::string>{"0","1"});
    inv.undo();
    CHECK(inv.lines() == std::vector<std::string>{"0"});
    inv.undo();
    CHECK(inv.lines() == std::vector<std::string>{});
    inv.undo();
    CHECK(inv.lines() == std::vector<std::string>{});
    CHECK(inv.changed() == false);

    // REDO ALL
    inv.redo();
    CHECK(inv.lines() == std::vector<std::string>{"0"});
    inv.redo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1"});
    inv.redo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1", "2"});
    inv.redo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1", "2"});

    inv.undo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1"});
    inv.insert(2, "2n");
    CHECK(inv.lines() == std::vector<std::string>{"0", "1", "2n"});
    inv.redo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1", "2n"});

    inv.undo();
    inv.undo();
    inv.undo();
    CHECK(inv.size() == 0);
    inv.redo();
    inv.redo();
    inv.redo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1", "2n"});

    inv.remove(0);
    CHECK(inv.lines() == std::vector<std::string>{"1", "2n"});
    inv.remove(1);
    CHECK(inv.lines() == std::vector<std::string>{"1"});
    inv.undo();
    inv.undo();
    CHECK(inv.lines() == std::vector<std::string>{"0", "1", "2n"});
    inv.undo();
    inv.undo();
    inv.undo();
    inv.undo();
    inv.undo();
    inv.undo();
    CHECK(inv.lines().empty());
    CHECK(inv.changed() == false);
}
