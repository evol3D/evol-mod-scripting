-- evol's internal types
Vec3 = {}
function Vec3:new(nx, ny, nz)
  -- creating an instance of the class
  out = {}
  setmetatable(out, self)
  self.__index = self

  -- initialization
  out.x = nx or 0
  out.y = ny or nx or 0
  out.z = nz or nx or 0

  return out
end

Vec3.__add = function(v1, v2)
  return Vec3:new(
    v1.x + v2.x,
    v1.y + v2.y,
    v1.z + v2.z)
end

Vec3.__sub = function(v1, v2)
  return Vec3:new(
    v1.x - v2.x,
    v1.y - v2.y,
    v1.z - v2.z)
end

Vec3.add = function(self, other)
  self.x = self.x + other.x
  self.y = self.y + other.y
  self.z = self.z + other.z
end

Vec3.sub = function(self, other)
  self.x = self.x - other.x
  self.y = self.y - other.y
  self.z = self.z - other.z
end


Vec4 = {}
function Vec4:new(nx, ny, nz, nw)
  -- creating an instance of the class
  out = {}
  setmetatable(out, self)
  self.__index = self

  -- initialization
  out.x = nx or 0
  out.y = ny or nx or 0
  out.z = nz or nx or 0
  out.w = nw or nx or 0

  return out
end

Vec4.__add = function(v1, v2)
  return Vec4:new(
    v1.x + v2.x,
    v1.y + v2.y,
    v1.z + v2.z,
    v1.w + v2.w)
end

Vec4.__sub = function(v1, v2)
  return Vec4:new(
    v1.x - v2.x,
    v1.y - v2.y,
    v1.z - v2.z,
    v1.w - v2.w)
end

Vec4.add = function(self, other)
  self.x = self.x + other.x
  self.y = self.y + other.y
  self.z = self.z + other.z
  self.w = self.w + other.w
end

Vec4.sub = function(self, other)
  self.x = self.x - other.x
  self.y = self.y - other.y
  self.z = self.z - other.z
  self.w = self.w - other.w
end






-- Entity Stuff

Entities = {}
setmetatable(Entities, Entities)

function Entities:__index(key)
  return rawget(self, key) or Entity:new(key)
end

Entity = {}
ComponentGetters = {}
EntityMemberGetters = {}
EntityMemberSetters = {}

function Entity:getID()
  return self.entityID
end

function Entity:setID(newID)
  self.entityID = newID
end

function Entity:new(id)
  new = {}
  setmetatable(new, self)
  -- self.__index = self

  new:setID(id)

  Entities[id] = new

  return new
end

function Entity:getComponent(cmp)
  return ComponentGetters[cmp](self.entityID)
end

function Entity:__index(key)
  if EntityMemberGetters[key] == nil then
    return rawget(self, key) or rawget(getmetatable(self), key)
  else
    return EntityMemberGetters[key](self.entityID)
  end
end

function Entity:__newindex(key, val)
  if EntityMemberSetters[key] == nil then
    rawset(self, key, val)
  else
    EntityMemberSetters[key](self.entityID, val)
  end
end
