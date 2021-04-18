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







-- Entity Stuff

Entities = {}

Entity = {}

ComponentGetters = {}

function Entity:getID()
  return self.entityID
end

function Entity:setID(newID)
  self.entityID = newID
end

function Entity:new(id)
  new = {}
  setmetatable(new, self)
  self.__index = self

  new:setID(id)

  Entities[id] = new

  return new
end

function Entity:getComponent(cmp)
  return ComponentGetters[cmp](self.entityID)
end
