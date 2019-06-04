#include <map>
#include "cocos2d.h"
#include "Snake.h"

struct RebornData {
	SnakeType type;
	cocos2d::Color3B color;
};

class archorManager {
public:
	static archorManager* GetInstance();

	void SetBodyParent(cocos2d::Node* parent);
	int Createarchor(cocos2d::Vec2 Pos, cocos2d::Vec2 Dir,  archor type);
	int GetPlayerId();
	Snake* GetSnake(int id) {
		if (_snakes.find(id) == _snakes.end()) {
			return nullptr;
		}
		return _snakes[id];
	}
	const std::map<int, Snake*>& GetSnakes() { return _snakes; }

	void Update(float dt);

private:
	SnakeManager() :
		_parent(nullptr)
	{}

	void RemoveDeadSnakes(float dt);
	void UpdateSnakes(float dt);

private:
	cocos2d::Node*                                  _parent;
	std::vector<RebornData>                      _reviveInfo;
	std::map<int, Snake*>                         _snakes;
	static SnakeManager*                        _instance;
};

#endif
