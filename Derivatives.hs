module Regexp where

import Prelude hiding (seq)

data Regexp = Empty
              | Epsilon
              | Char Char
              | Star Regexp
              | Alt Regexp Regexp
              | Concat Regexp Regexp
            deriving (Eq, Ord)

match :: Regexp -> String -> Bool
match r s = nullable (derivative' s r) == Epsilon

derivative :: Char -> Regexp -> Regexp
derivative a Empty = Empty 
derivative a Epsilon = Empty
derivative a (Char b) | a == b = Epsilon 
derivative a (Char b) = Empty
derivative a (Alt s t) = Alt (derivative a s) (derivative a t)
derivative a (Star s) = Concat (derivative a s) (Star s) 
derivative a (Concat s t) = Alt (Concat (derivative a s) t) (Concat (nullable s) (derivative a t))

intersect :: Regexp -> Regexp -> Regexp
intersect Empty _ = Empty 
intersect _ Empty = Empty 
intersect Epsilon Epsilon = Epsilon
intersect _ _ = error "Sorry, intersect is unimplemented for these Regexp types"

union :: Regexp -> Regexp -> Regexp
union Epsilon _ = Epsilon 
union _ Epsilon = Epsilon  
union Empty Empty = Empty
union _ _ = error "Sorry, intersect is unimplemented for these Regexp types"

nullable :: Regexp -> Regexp
nullable Empty = Empty
nullable Epsilon = Epsilon
nullable (Char a) = Empty
nullable (Concat s t) = intersect (nullable s) (nullable t)
nullable (Alt s t) = union (nullable s) (nullable t)
nullable (Star s) = Epsilon

derivative' :: String -> Regexp -> Regexp
derivative' (c:cs) r = derivative' cs (derivative c r)
derivative' [] r = r

r1 = Concat (Star (Concat (Char 'a') (Char 'b')) ) (Char 'a')
