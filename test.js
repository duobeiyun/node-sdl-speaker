import test from 'ava';
import Speaker from './index';
import { Buffer } from 'buffer';

Speaker.init({}, err => {
  console.error(err);
})

test('mix', t => {
  let a = Buffer.alloc(6, 1000);
  let b = Buffer.alloc(6, 1000);
  let c = Buffer.alloc(6, 1000);
  let res = Speaker.mix(6, a, b, c, c);
  t.deepEqual(a, res);
})
