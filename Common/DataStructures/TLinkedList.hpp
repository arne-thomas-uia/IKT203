#pragma once
#include <stdexcept> // For std::runtime_error
#include <string>    // For std::to_string
#include <functional> // For std::function
#include "../DebugUtils.h" // For debugText function
#include "TCollection.hpp" // Your base collection class
#include "TNode.hpp" // Your node class

/**
* @file   TLinkedList.hpp
* @brief  Dear student, this file contains the definition of the TLinkedList class template, which serves as a doubly-linked list implementation for storing elements of type T.
*					You will create this as a template class typically holding methods for appending, prepending, searching, and iterating over elements.
*					Type of linked list is op to you, you must clean (delete) your data yourself so be wize about the type of T you choose to use.
*         I recommend to implenet a "isOwner" boolean variable to determine if the list is responsible for deleting the data or not.
*					Please ask for help in Discord if you have any questions or need assistance with the implementation.
* @tparam T The type of elements stored in the linked list.
*/

