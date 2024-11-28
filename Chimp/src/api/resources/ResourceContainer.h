#pragma once

#include "Resource.h"
#include "api/graphics/shaders/ShaderTypes.h"

namespace Chimp {
	typedef std::string TextureResourcePath;
	typedef ShaderFilePaths ShaderResourcePath;
	typedef std::string ModelResourcePath;

	template <typename K, typename T>
	class ResourceContainer {
	public:
		ResourceContainer(std::function<Resource<T>(const K&)> loadResourceFunc) : m_LoadResourceFunc(loadResourceFunc) {}
		virtual ~ResourceContainer() = default;

		// Depend on a resource, this loads it if it hasn't been loaded yet and increments the reference count
		T& Depend(const K& path) {
			if (m_Resources.find(path) == m_Resources.end()) {
				m_Resources[path] = m_LoadResourceFunc(path);
				assert(m_Resources[path].RefCount == 0);
			}

			m_Resources[path].RefCount++;
			return *m_Resources[path].Data;
		}

		// Get a resource, does bad stuff if it doesn't exist, only call this if you are 100% sure it exists
		[[nodiscard]] T& Get(const K& path) {
			assert(m_Resources.find(path) != m_Resources.end());
			return *m_Resources[path].Data;
		}

		// Release a resource, this decrements the reference count
		void Release(const K& path) {
			if (m_Resources.find(path) == m_Resources.end()) {
				return;
			}

			m_Resources[path].RefCount--;
		}

		// Unload all resources that have a reference count of 0
		void UnloadUnused() {
			for (auto it = m_Resources.begin(); it != m_Resources.end();) {
				if (it->second.RefCount <= 0) {
					it = m_Resources.erase(it);
				}
				else {
					++it;
				}
			}
		}
	private:
		std::unordered_map<K, Resource<T>> m_Resources;
		std::function<Resource<T>(const K&)> m_LoadResourceFunc;
	};
}