#pragma once

#include <functional>
#include <map>
#include <unordered_map>


/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data) = 0;
	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T>& observer, int priority = 0) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(IObserver<T>& observer) = 0;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T>
{
public:
	typedef IObserver<T> ObserverType;

	void RegisterObserver(ObserverType& observer, int priority = 0) override
	{
		if (m_observerPriorities.find(&observer) != m_observerPriorities.end())
		{
			return;
		}

		auto it = m_priorityObservers.emplace(priority, &observer);
		try
		{
			m_observerPriorities.emplace(&observer, priority);
		}
		catch (...)
		{
			m_priorityObservers.erase(it);
			throw;
		}
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();

		auto observersCopy = m_priorityObservers;

		for (auto& [priority, observers] : copy)
		{
			for (auto* observer : observers)
			{
				observer->Update(data);
			}
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		auto it = m_observerPriorities.find(&observer);
		if (it == m_observerPriorities.end())
		{
			return;
		}

		int priority = it->second;
		m_observerPriorities.erase(it);
		//разобратьс equal range
		auto range = m_priorityObservers.equal_range(priority);
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			if (iter->second == &observer)
			{
				m_priorityObservers.erase(iter);
				break;
			}
		}
	}

protected:
	// Классы-наследники должны перегрузить данный метод, 
	// в котором возвращать информацию об изменениях в объекте
	virtual T GetChangedData()const = 0;

private:
	// разобраться чем отличается от map
	std::map<int, std::unordered_set<ObserverType*>, std::greater<int>> m_priorityObservers;
	std::unordered_map<ObserverType*, int> m_observerPriorities;
};
