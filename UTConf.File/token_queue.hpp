#pragma once

#include "token.hpp"
#include <queue>
#include <memory>

namespace utconf {

class token_queue {
public:
	explicit token_queue() noexcept : _prev(nullptr) {};
	explicit token_queue(const token_queue& other) noexcept : _prev(other._prev.get()), _queue(other._queue) {}

	virtual ~token_queue() = default;

public:
	const std::shared_ptr<token> next_token() {
		if (is_empty()) {
			return nullptr;
		}

		std::shared_ptr<token> tok = std::make_shared<token>(_queue.front());
		_prev.reset();
		_prev = std::make_shared<token>(*tok);
		_queue.pop();
		return tok;
	}

	void push_back(token tok) {
		_queue.push(tok);
	}

	token& front_tok() {
		return _queue.front();
	}

	token& back_tok() {
		return _queue.back();
	}

	void pop_front() {
		_queue.pop();
	}

	const std::shared_ptr<token> get_prev() const {
		return _prev;
	}

	bool is_empty() const {
		return _queue.empty();
	}

	void clear() {
		while (!_queue.empty()) {
			_queue.pop();
		}
	}

private:
	std::shared_ptr<token> _prev;
	std::queue<token> _queue;
};

};
