



function validaCPF(valor)
    -- d1
  if(#valor ~=11)then
      return nil
  end
  local m = 10
  local sum = 0
  local d1 = 0
  for i = 1, #valor-2 do
    local aux = tonumber(valor:sub(i,i))
    sum = sum + aux * m
    m = m - 1
  end
  
  local r = sum%11
  
  if (r == 0 or r == 1)then
    d1 = 0
    if(tonumber(valor:sub(10, 10)) ~= d1) then
      return 1
    end
    else
      d1 = 11 - r
      if (tonumber(valor:sub(10, 10)) ~= d1) then
        return 1
      end
  end

  -- d2  
  local m = 10
  local sum = 0
  local d2 = 0
  for i = 2, #valor-1 do
    local aux = tonumber(valor:sub(i,i))

    sum = sum + aux * m
    m = m - 1
  end
  local r = sum%11
  if (r == 0 or r == 1)then
    d2 = 0
    if(tonumber(valor:sub(11, 11)) ~= d2) then
      return 1
    end
    else
      d2 = 11 - r
      if (tonumber(valor:sub(11, 11)) ~= d2) then
        return 1
      end
  end
    --d1 e d2 verified
    if(tonumber(valor:sub(10, 10)) == d1 and tonumber(valor:sub(11, 11)) == d2)then
      return 0
  else
    return 1
  end

end
function getCPF(valor)
  -- Split the string into three parts and format them
  local part1 = valor:sub(1, 3)
  local part2 = valor:sub(4, 6)
  local part3 = valor:sub(7, 9)
  local part4 = valor:sub(10, 11)

  return string.format("%s.%s.%s-%s", part1, part2, part3, part4)
end



--formata para brasil format
function getData(valor)
    local y, m, d = valor:match("(%d+)-(%d+)-(%d+)")

    if y and m and d then
        -- troca ano de lugar com o dia
        return string.format("%s/%s/%s", d, m, y)
    else
        return nil 
    end
end

function validaData(valor)
    -- Define the ISO 8601 date pattern for year, month, and day
    local pattern = "^%d%d%d%d%-%d%d%-%d%d$"

    -- Attempt to match the string against the pattern
    if valor:match(pattern) then
        return true
    else
        return false
    end
end





