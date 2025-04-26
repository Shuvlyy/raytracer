#pragma once

#define IF_T_IS_TYPE(t) if constexpr (std::is_same_v<T, t>)

#include "yml/Exception/Exceptions/InvalidNodeType.hpp"
#include "yml/Exception/Exceptions/UnknownNodeType.hpp"

#include <string>
#include <unordered_map>

namespace yml
{

    struct Node; /// Forward declaration so that Tree can reference it


    /**
     * @brief   Represents a container of named child nodes.
     *
     * The Tree class manages a collection of Node objects, enabling
     * hierarchical storage and retrieval by name.
     */
    class Tree final
    {
    public:
        /**
         * @brief   Adds a Node to the tree.
         *
         * @param   node    Reference to the Node to be added
         */
        void addNode(Node& node);

        /**
         * @brief   Retrieves all child nodes stored in the tree.
         *
         * @returns Const reference to an unordered map containing all child
         *          Nodes.
         */
        [[nodiscard]] const std::unordered_map<std::string, Node>&
            getNodes() const { return this->_children; }

        /**
         * @brief   Accesses a child node by its name.
         *
         * This operator allows access to a specific Node in the tree by
         * providing its name.
         *
         * @param   name    The name of the Node to access
         * @return  A reference to the corresponding Node
         */
        Node& operator[](const std::string& name);

        /**
         * @brief   Accesses a child node by its name (const version).
         *
         * This operator allows read-only access to a specific Node in the tree
         * by providing its name.
         *
         * @param   name    The name of the Node to access
         * @return  A const reference to the corresponding Node
         */
        const Node& operator[](const std::string& name) const;

        /**
         * @brief   Accesses a child node by its index.
         *
         * This operator allows access to a Node based on its position in the
         * insertion order. Since the underlying container is an unordered_map,
         * the order is not guaranteed to be stable across runs. FIXME!
         *
         * @param   index   The zero-based index of the Node to access
         * @returns A reference to the corresponding Node
         */
        Node& operator[](size_t index);

        /**
         * @brief   Accesses a child node by its index (const version).
         *
         * This operator allows read-only access to a Node based on its
         * position in the insertion order.
         *
         * @param   index   The zero-based index of the Node to access
         * @return  A const reference to the corresponding Node
         */
        const Node& operator[](size_t index) const;

    private:
        std::unordered_map<std::string, Node> _children;
    };


    namespace node
    {

        /**
         * @brief   Represents the type of value or structure stored in a Node.
         *
         * This enum is used to indicate how a Node's value should be
         * interpreted.
         * Parsing or type conversion depends on this information.
         */
        enum Type
        {
            STRING,
            INTEGER,
            DOUBLE,
            BOOLEAN,
            OBJECT,     // Has children
            LIST,       // A list of items (array)
            UNKNOWN     // Fallback
        };

    }

    /**
     * @brief   Represents a node in the YAML tree.
     *
     * A Node can hold a name, an optional value, and its own subtree
     * (children).
     * It also supports detecting list items (marked by "- " at the beginning).
     */
    struct Node
    {
        std::string name;
        std::string value;
        bool isList = false;
        node::Type type = node::UNKNOWN;
        Tree children;

        /**
         * @brief   Constructs a new Node object.
         *
         * Automatically detects list-style items (starting with "- ").
         *
         * @param   name    Name of the node
         * @param   value   Value of the node (can be empty)
         */
        Node(
            const std::string& name,
            const std::string& value
        );

        template<typename T = std::string>
        T as()
            const
        {
            IF_T_IS_TYPE(std::string) {
                return this->value;
            }
            IF_T_IS_TYPE(int) {
                if (this->type != node::INTEGER) {
                    throw exception::InvalidNodeType(this->name, "INT");
                }
                return std::stoi(value);
            }
            IF_T_IS_TYPE(double) {
                if (this->type != node::DOUBLE && this->type != node::INTEGER) {
                    throw exception::InvalidNodeType(this->name, "FLOAT");
                }
                return std::stod(value);
            }
            IF_T_IS_TYPE(bool) {
                if (this->type != node::BOOLEAN) {
                    throw exception::InvalidNodeType(this->name, "BOOLEAN");
                }
                return this->value == "true" || this->value == "1";
            }

            throw exception::UnknownNodeType(this->name);
        }

        /**
         * @brief   Prints the Node and its children recursively to the
         *          standard output.
         *
         * @param   depth   Current indentation level (used internally for
         *                  nested structures).
         */
        void dump(size_t depth = 0) const;

        /**
         * @brief   Accesses a child node by its name.
         *
         * This operator allows access to a direct child Node by name if the
         * current Node has children (i.e., is an OBJECT or LIST).
         *
         * @param   name    The name of the child Node to access
         * @returns A reference to the corresponding child Node
         */
        Node& operator[](const std::string& name) { return this->children[name]; }

        /**
         * @brief   Accesses a child node by its name (const version).
         *
         * This operator allows read-only access to a direct child Node by
         * name.
         *
         * @param   name    The name of the child Node to access
         * @returns A const reference to the corresponding child Node
         */
        const Node& operator[](const std::string& name) const { return this->children[name]; }

        /**
         * @brief   Accesses a child node by its index.
         *
         * This operator allows access to a child Node by position in the
         * insertion order. Useful when iterating over list-type nodes.
         *
         * @param   index   The zero-based index of the child Node to access
         * @return  A reference to the corresponding child Node
         */
        Node& operator[](const size_t index) { return this->children[index]; }

        /**
         * @brief   Accesses a child node by its index (const version).
         *
         * This operator allows read-only access to a child Node by position.
         *
         * @param   index   The zero-based index of the child Node to access
         * @return  A const reference to the corresponding child Node
         */
        const Node& operator[](const size_t index) const { return this->children[index]; }

    private:
        void detectList();
        void detectType();
    };

}
