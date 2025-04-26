#include "Node.hpp"
#include "Yml.hpp"

#include <iostream>
#include <sstream>

namespace yml
{

    void
    Tree::addNode(Node &node)
    {
        this->_children.insert({ node.name, node });
    }

    Node &
    Tree::operator[]
    (
        const std::string &name
    )
    {
        auto it = this->_children.find(name);

        if (it == this->_children.end()) {
            throw std::out_of_range("No such node: " + name);
        }
        return it->second;
    }

    const Node&
    Tree::operator[]
    (
        const std::string &name
    )
        const
    {
        auto it = this->_children.find(name);

        if (it == this->_children.end()) {
            throw std::out_of_range("No such node: " + name);
        }
        return it->second;
    }

    Node &
    Tree::operator[]
    (
        const size_t index
    )
    {
        if (index >= this->_children.size()) {
            throw std::out_of_range("Index out of range in Tree");
        }

        auto it = this->_children.begin();
        std::advance(it, index);
        return it->second;
    }

    const Node &
    Tree::operator[]
    (
        size_t index
    )
        const
    {
        if (index >= this->_children.size()) {
            throw std::out_of_range("Index out of range in Tree");
        }

        auto it = this->_children.begin();
        std::advance(it, index);
        return it->second;
    }

    Node::Node
    (
        const std::string &name,
        const std::string &value
    )
    {
        this->name = name;
        this->value = value;

        this->detectList();
        this->detectType();
    }

    void
    Node::dump(const size_t depth)
        const
    {
        std::cout << std::string(depth * YML_NESTING_SPACES, ' ');

        if (this->isList) {
            std::cout << "- " << this->name;
            if (!this->children.getNodes().empty()) {
                std::cout << ":";
            }
            std::cout << std::endl;
        }
        else {
            std::cout << this->name << ":";
            if (!this->value.empty()) {
                std::cout << " " << this->value;
            }
            std::cout << std::endl;
        }

        for (const auto& [_, node] : this->children.getNodes()) {
            node.dump(depth + 1);
        }
    }

    void
    Node::detectList
    ()
    {
        if (this->name.size() < 2) {
            return;
        }

        this->isList = (this->name[0] == '-' && this->name[1] == ' ');

        if (this->isList) {
            this->name = this->name.substr(2);
        }
    }

    void
    Node::detectType
    ()
    {
        if (!this->children.getNodes().empty()) {
            this->type = this->isList ? node::LIST : node::OBJECT;
            return;
        }

        if (value.empty()) {
            this->type = node::STRING;
            return;
        }

        if (value == "true" || value == "false") {
            type = node::BOOLEAN;
        }
        else if (value.find('.') != std::string::npos) {
            std::istringstream iss(value);
            double d;
            if (iss >> d) {
                type = node::DOUBLE;
            } else {
                type = node::STRING;
            }
        }
        else {
            std::istringstream iss(value);
            int i;
            if (iss >> i) {
                type = node::INTEGER;
            } else {
                type = node::STRING;
            }
        }
    }

}
