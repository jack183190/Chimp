#pragma once

#include "stdafx.h"

namespace Chimp {
	typedef unsigned int EventListener;

	template <typename EventType, typename Event>
	class EventHandler {
	public:
		class Broadcaster {
			friend class EventHandler<EventType, Event>;
		private:
			Broadcaster(EventHandler* handler) : m_Handler(handler) {}
		public:
			// Broadcast an event to the event handler
			void Broadcast(EventType type, const Event& event) {
				if (!m_Handler) [[unlikely]] {
					assert(false); // Event handler has been destroyed, can't broadcast event
					return;
					}
				m_Handler->Broadcast(type, event);
			}
		private:
			EventHandler<EventType, Event>* m_Handler;
		};
		friend class Broadcaster;
	public:
		// Create an event handler, can be used to broadcast events and sub/unsub listeners
		// broadcaster - empty unique_ptr which will be filled with a broadcaster for this event handler
		EventHandler(std::shared_ptr<Broadcaster> &broadcaster) {
			broadcaster = std::shared_ptr<Broadcaster>(new Broadcaster(this));
			m_UnusedBroadcaster = broadcaster;
		}

		~EventHandler() {
			m_UnusedBroadcaster->m_Handler = nullptr;
		}
	public:
		// Subscribe to an event type
		// type - the type of event to subscribe to
		// callback - the function to call when the event is broadcast
		// returns a listener id which can be used to unsubscribe
		EventListener Subscribe(EventType type, const std::function<void(const Event*)> &callback) {
			m_LastListenerId++;
			m_Subscribers[type][m_LastListenerId] = callback;
			return m_LastListenerId;
		}

		// Unsubscribe from an event type
		// listener - the listener id to unsubscribe
		void Unsubscribe(EventListener listener) {
			for (auto& [type, subscribers] : m_Subscribers) {
				subscribers.erase(listener);
			}
		}

	private:
		// Broadcast an event to all subscribers
		void Broadcast(EventType type, const Event& event) {
			for (auto& [listener, callback] : m_Subscribers[type]) {
				callback(&event);
			}
		}

	private:
		EventListener m_LastListenerId = 0;
		std::unordered_map<EventType, std::unordered_map<EventListener, std::function<void(const Event*)>>> m_Subscribers;
		std::shared_ptr<Broadcaster> m_UnusedBroadcaster; // Used to keep the broadcaster alive
	};


	template <typename EventType, typename Event>
	struct EventHandlerAndBroadcaster {
		std::unique_ptr<EventHandler<EventType, Event>> Handler;
		std::shared_ptr<typename EventHandler<EventType, Event>::Broadcaster> Broadcaster;
	};
}